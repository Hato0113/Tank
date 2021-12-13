//----------------------------------
//	ピクセルシェーダーベース
//-- author --
//	HatoriMasashi
//----------------------------------

//-- include --
#define _CRT_SECURE_NO_WARNINGS
#include "PixelBase.h"
#include "MySystem\DirectX.h"

PixelBase::PixelBase()
{
	m_Shader = nullptr;
}

PixelBase::~PixelBase()
{
	if (m_Shader)
	{
		m_Shader->Release();
		m_Shader = nullptr;
	}
}

/*
	自身のバインド
*/
void PixelBase::Bind()
{
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	pDC->PSSetShader(m_Shader, nullptr, 0);
}

/*
	シェーダーロード
	引数 : ファイル名
*/
HRESULT PixelBase::Make(std::string fileName)
{
	//-- シェーダーロード --
	HRESULT hr = S_OK;
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (!fp) { return E_FAIL; }

	//-- ファイルの中身をメモリに読み込み --
	char* pData = nullptr;
	//-- ファイルのデータサイズを調べる --
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);	//移動量検出
	fseek(fp, 0, SEEK_SET);
	//-- メモリ確保して読み込み --
	pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();
	hr = pDevice->CreatePixelShader(pData, fileSize, nullptr, &m_Shader);
	if (FAILED(hr)) { 
		if (pData) delete[] pData;
		return hr;
	}

	if (pData)
		delete[] pData;

	return hr;
}
