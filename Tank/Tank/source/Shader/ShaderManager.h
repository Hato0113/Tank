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
	static ShaderManager& GetInstance()
	{
		static ShaderManager inst;
		return inst;
	}

private:
	void Set(std::string name, VertexBase*);
	void Set(std::string name, PixelBase*);
	void Set(std::string name, ConstantBuffer*);
	void Set(std::string name, ShaderTexture*);

public:
	void AllResourceBind();	//全バインド / 一回だけ呼ぶ?
	void TextureResourceBind();
	void ConstantWrite(std::string name, void* data);

	void Init();	//初期化
	void Uninit();	//シェーダー,定数バッファ解放

	void BindVS(std::string name);
	void BindPS(std::string name);

private:
	std::unordered_map<std::string, VertexBase*> VertexMap;	//頂点シェーダーマップ
	std::unordered_map<std::string, PixelBase*> PixelMap;	//ピクセルシェーダーマップ
	std::unordered_map<std::string, ConstantBuffer*> ConstantMap;	//定数バッファマップ
	std::unordered_map<std::string, ShaderTexture*> TextureMap;	//テクスチャ
};


