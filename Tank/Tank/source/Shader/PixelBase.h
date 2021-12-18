//----------------------------------
//	ピクセルシェーダーベース
//-- author --
//	HatoriMasashi
//----------------------------------
#pragma once

//-- include --
#include <d3d11.h>
#include <string>

//-- クラス定義 --
class PixelBase final
{
public:
	PixelBase();
	~PixelBase();
public:
	/*
		バインド関数
		パラメータ無し
	*/
	void Bind();
	/*
		シェーダー作成
		引数 : ファイル名
	*/
	HRESULT Make(std::string fileName);	//シェーダー作成関数

private:
	ID3D11PixelShader* m_Shader;	//シェーダー本体
};