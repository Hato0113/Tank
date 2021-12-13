//------------------------------
//	テクスチャマネージャー
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include <unordered_map>
#include "TextureID.h"
#include <d3d11.h>

//-- クラス定義 --
class TextureManager
{
public:
	static void Init();
	static void Uninit();
	static void Load(TextureID, const char* fileName);
	static ID3D11ShaderResourceView* Get(TextureID);
private:
	static TextureManager* pTexManager;	//自身オブジェクトポインタ
	std::unordered_map<TextureID, ID3D11ShaderResourceView*> m_pTexMap;
};


