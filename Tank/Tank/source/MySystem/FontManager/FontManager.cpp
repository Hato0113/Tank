//----------------------
//	FontManager
//-- author --
//	HatoriMasashi
//----------------------

//-- include --
#include "FontManager.h"
#include "Component\Polygon\Polygon.h"

LOGFONT FontManager::m_FontData;
HDC FontManager::m_hdc;
TEXTMETRIC FontManager::m_TextMetric;
GLYPHMETRICS FontManager::m_GM;
D3D11_TEXTURE2D_DESC FontManager::m_FontTextureDesc;
ID3D11Texture2D* FontManager::m_FontTexture = nullptr;
HFONT FontManager::m_Font;
HFONT FontManager::m_oldFont;
std::unordered_map<char, FontManager::FontInfo> FontManager::m_CharInfo;


void FontManager::Init()
{
	LoadFont();
}

void FontManager::Uninit()
{
	for (auto data : m_CharInfo)
	{
		data.second.m_pTex->Release();
	}

	m_CharInfo.clear();

	SelectObject(m_hdc, m_oldFont);
	DeleteObject(m_Font);

	DESIGNVECTOR design;
	RemoveFontResourceEx(
		TEXT("data/Font/ささふでフォント.ttf"),
		FR_PRIVATE,
		&design
	);
}

void FontManager::LoadFont()
{
	//-- フォントハンドル生成 --
	int fontSize = 64;
	int fontWeight = 64;

	//-- フォント読み込み --
	DESIGNVECTOR design;
	AddFontResourceEx(
		TEXT("data/Font/ささふでフォント.ttf"),
		FR_PRIVATE,
		&design
	);
	//-- データ形成 --
	m_FontData =
	{
		fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
		SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
		NULL
	};
	wcscpy_s(m_FontData.lfFaceName, L"ささふでフォント");
	m_Font = CreateFontIndirectW(&m_FontData);

	//-- 現在のウィンドウに適用 --
	m_hdc = GetDC(NULL);
	m_oldFont = (HFONT)SelectObject(m_hdc, m_Font);
}

ID3D11ShaderResourceView* FontManager::GetFont(char c)
{
	if (m_CharInfo.count(c))
	{
		return m_CharInfo[c].m_pTex;	//これまでに使っている
	}
	else
	{
		FontInfo temp;
		m_CharInfo.insert(std::make_pair(c, temp));
	}

	// フォントビットマップ取得
	UINT code = (UINT)c;
	const int gradFlag = GGO_GRAY4_BITMAP;
	int grad = 0;
	switch (gradFlag)
	{
	case GGO_GRAY2_BITMAP:
		grad = 4;
		break;
	case GGO_GRAY4_BITMAP:
		grad = 16;
		break;
	case GGO_GRAY8_BITMAP:
		grad = 64;
		break;
	}
	HDC hdc = m_hdc;
	GetTextMetrics(hdc, &m_TextMetric);
	CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };
	DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &m_GM, 0, NULL, &mat);
	BYTE* m_pMono = new BYTE[size];
	GetGlyphOutlineW(hdc, code, gradFlag, &m_GM, size, m_pMono, &mat);

	//フォントの幅と高さ
	int fontWidth = m_GM.gmCellIncX;
	int fontHeight = m_TextMetric.tmHeight;


	//フォントを書き込むテクスチャ作成
	ZeroMemory(&m_FontTextureDesc, sizeof(m_FontTextureDesc));
	m_FontTextureDesc.Width = fontWidth;
	m_FontTextureDesc.Height = fontHeight;
	m_FontTextureDesc.MipLevels = 1;
	m_FontTextureDesc.ArraySize = 1;
	m_FontTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_FontTextureDesc.SampleDesc.Count = 1;
	m_FontTextureDesc.SampleDesc.Quality = 0;
	m_FontTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_FontTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_FontTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_FontTextureDesc.MiscFlags = 0;
	m_FontTexture = nullptr;

	//-- デバイス取得 --
	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();
	ID3D11DeviceContext* pDeviceContext = DX::DirectXManager::GetInstance().GetDeviceContext();

	HRESULT hr = pDevice->CreateTexture2D(&m_FontTextureDesc, NULL, &m_FontTexture);
	// フォント情報をテクスチャに書き込む部分
	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = pDeviceContext->Map(
		m_FontTexture,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&hMappedResource);
	// ここで書き込む
	BYTE* pBits = (BYTE*)hMappedResource.pData;
	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
	int iOfs_x = m_GM.gmptGlyphOrigin.x;
	int iOfs_y = m_TextMetric.tmAscent - m_GM.gmptGlyphOrigin.y;
	int iBmp_w = m_GM.gmBlackBoxX + (4 - (m_GM.gmBlackBoxX % 4)) % 4;
	int iBmp_h = m_GM.gmBlackBoxY;
	int Level = 17;
	int x, y;
	DWORD Alpha, Color;
	memset(pBits, 0, hMappedResource.RowPitch * m_TextMetric.tmHeight);
	for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
	{
		for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
		{
			Alpha =
				(255 * m_pMono[x - iOfs_x + iBmp_w * (y - iOfs_y)])
				/ (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			memcpy(
				(BYTE*)pBits
				+ hMappedResource.RowPitch * y + 4 * x,
				&Color,
				sizeof(DWORD));
		}
	}
	pDeviceContext->Unmap(m_FontTexture, 0);
	//不要なので削除
	delete[] m_pMono;

	// ShaderResourceViewの情報を作成する
	ID3D11ShaderResourceView* pShader;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = m_FontTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = m_FontTextureDesc.MipLevels;

	//-- 情報保存 --
	m_CharInfo[c].Width = fontWidth;
	m_CharInfo[c].Height = fontHeight;
	m_CharInfo[c].OffSetX = iOfs_x;
	m_CharInfo[c].OffSetY = iOfs_y;
	m_CharInfo[c].BMPWidth = iBmp_w;
	m_CharInfo[c].BMPHeight = iBmp_h;

	//-- ShaderResourceViewへの変換 --
	pDevice->CreateShaderResourceView(m_FontTexture, &srvDesc, &pShader);
	m_FontTexture->Release();
	m_CharInfo[c].m_pTex = pShader;
	return pShader;
}

std::vector<CPolygon*> FontManager::CreateString(Object* parent, std::string str, DirectX::XMFLOAT2 pos, float scale, DirectX::XMFLOAT4 color,bool isFront)
{
	std::vector<CPolygon*> polygonList;
	const int Margin = 0;
	parent->transform->SetTag("String");
	auto offset = pos;
	for (auto s : str)
	{
		auto poly = parent->AddComponent<CPolygon>();
		poly->SetTex(FontManager::GetFont(s));
		poly->SetPos(offset);
		poly->SetSize({ m_CharInfo[s].Width * scale,m_CharInfo[s].Height * scale });
		poly->SetColor({ color.x,color.y,color.z });
		poly->SetAlpha(color.w);
		if(isFront)
			poly->SetLayer(Layer::Front2D);

		polygonList.push_back(poly);

		offset.x += m_CharInfo[s].Width * scale + Margin / 2.0f;
	}
	return polygonList;
}