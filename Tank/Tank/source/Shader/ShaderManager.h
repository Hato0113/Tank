//-----------------------------
//	シェーダーマネージャー
//-- author --
//	HatoriMasashi
//-----------------------------
#pragma once

//-- include --
#include <d3d11.h>
#include <unordered_map>
#include <vector>
#include "VertexBase.h"
#include "PixelBase.h"
#include "ConstantBuffer.h"
#include "ShaderTexture.h"

//-- クラス定義 --
class ShaderManager
{
private:
	ShaderManager();
	~ShaderManager() = default;
public:
	/*
		シングルトン
		インスタンスゲット関数
		戻り値 : ShaderManagerオブジェクト
	*/
	static ShaderManager& GetInstance()
	{
		static ShaderManager inst;
		return inst;
	}

	/*
		データセット関数
		引数 : 登録する名前,データへのポインタ
	*/
private:
	void Set(std::string name, VertexBase*);
	void Set(std::string name, PixelBase*);
	void Set(std::string name, ConstantBuffer*);
	void Set(std::string name, ShaderTexture*);

public:
	/*
		定数バッファバインド
		パラメータ無し
	*/
	void ConstantBufferBind();
	/*
		テクスチャリソースバインド
		パラメータ無し
	*/
	void TextureResourceBind();
	/*
		定数バッファ書き込み
		引数 : 定数バッファ,書き込みデータ
	*/
	void ConstantWrite(std::string name, void* data);

	/*
		初期化
		パラメータ無し
	*/
	void Init();
	/*
		シェーダー,バッファ解放
		パラメータ無し
	*/
	void Uninit();

	/*
		頂点シェーダーバインド
		引数 : シェーダー名
	*/
	void BindVS(std::string name);
	/*
		ピクセルシェーダーバインド
		引数 : シェーダー名
	*/
	void BindPS(std::string name);

private:
	std::unordered_map<std::string, VertexBase*> VertexMap;	//頂点シェーダーマップ
	std::unordered_map<std::string, PixelBase*> PixelMap;	//ピクセルシェーダーマップ
	std::unordered_map<std::string, ConstantBuffer*> ConstantMap;	//定数バッファマップ
	std::unordered_map<std::string, ShaderTexture*> TextureMap;	//テクスチャ
};


