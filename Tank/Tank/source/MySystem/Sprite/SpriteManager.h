//------------------------------
//	スプライトマネージャー
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include <iostream>
#include "SpriteType.h"
#include <unordered_map>
#include <string>
#include "Objects\Object.h"
#include "MySystem\Texture\TextureID.h"

//-- クラス定義 --
/*
	スプライトを.csvファイル情報を読み込んで2Dオブジェクトとして生成
*/
class SpriteManager
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
		データロード
		引数 : スプライトタイプ,ファイル名,テクスチャID,テクスチャサイズ
	*/
	static bool Load(SpriteType, std::string fileName,TextureID id,int texWidth = 512,int texHeight = 512);
	/*
		オブジェクト生成
		引数 : スプライトタイプ,アイテム名,座標
		戻り値 : オブジェクトポインタ
	*/
	static Object* Make(SpriteType, std::string ItemName, DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f });

private:
	struct SpriteInfo
	{
		std::string name;
		int u; 
		int v;
		int width;
		int height;
	};
	struct TexInfo
	{
		int Width;
		int Height;
		TextureID TexID;
	};
	static SpriteManager* pSpriteManager;
	std::unordered_map<SpriteType, std::vector<SpriteInfo*>> SpriteMap;
	std::unordered_map<SpriteType, TexInfo> TexInfoMap;
};



