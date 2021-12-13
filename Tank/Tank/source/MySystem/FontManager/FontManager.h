//----------------------
//	FontManager
//-- author --
//	HatoriMasashi
//----------------------
#pragma once

//-- include --
#include "MySystem\DirectX.h"
#include <unordered_map>
#include "Objects\Object.h"

//-- クラス定義 --
class FontManager
{
public:
	static void Init();
	static void Uninit();

private:
	struct FontInfo
	{
		int Width;
		int Height;
		int BMPWidth;
		int BMPHeight;
		int OffSetX;
		int OffSetY;
		ID3D11ShaderResourceView* m_pTex;
	};

public:
	static void LoadFont();
	static ID3D11ShaderResourceView* GetFont(char c);
	static void CreateString(Object* parent, std::string, DirectX::XMFLOAT2, float scale = 1.0f,
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

private:

private:
	static LOGFONT m_FontData;
	static HDC m_hdc;
	static TEXTMETRIC m_TextMetric;
	static GLYPHMETRICS m_GM;
	static D3D11_TEXTURE2D_DESC m_FontTextureDesc;
	static ID3D11Texture2D* m_FontTexture;
	static HFONT m_Font;
	static HFONT m_oldFont;
	static std::unordered_map<char, FontManager::FontInfo> m_CharInfo;

};

