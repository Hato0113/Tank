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
	/*
		初期化
		パラメータ無し
	*/
	static void Init();
	/*
		終了処理
		パラメータ無し
	*/
	static void Uninit();
	/*
		テクスチャロード
		引数 : テクスチャID,ファイル名
	*/
	static void Load(TextureID, const char* fileName);
	/*
		テクスチャ取得
		引数 : テクスチャID
		戻り値 : リソースビュー
	*/
	static ID3D11ShaderResourceView* Get(TextureID);
private:
	static TextureManager* pTexManager;	//自身オブジェクトポインタ
	std::unordered_map<TextureID, ID3D11ShaderResourceView*> m_pTexMap;
};


