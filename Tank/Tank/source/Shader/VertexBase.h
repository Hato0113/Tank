//----------------------------------
//	頂点シェーダーベース
//-- author --
//	HatoriMasashi
//----------------------------------
#pragma once

//-- include --
#include <d3d11.h>
#include <string>

//-- クラス定義 --
class VertexBase abstract
{
public:
	VertexBase();
	virtual ~VertexBase();
public:
	/*
		バインド関数
		パラメータ無し
	*/
	virtual void Bind() final;

	/*
		シェーダー作成
		引数 : ファイル名
		戻り値 : 結果
	*/
	virtual HRESULT Make(std::string) = 0;

protected:
	ID3D11VertexShader* m_Shader;	//シェーダー本体
	ID3D11InputLayout* m_Layout;	//レイアウト
};

