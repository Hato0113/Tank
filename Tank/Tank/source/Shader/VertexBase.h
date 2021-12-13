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
	virtual void Bind() final;	//自身をデバイスに結びつけさせる
	virtual HRESULT Make(std::string) = 0;	//シェーダー作成関数

protected:
	ID3D11VertexShader* m_Shader;	//シェーダー本体
	ID3D11InputLayout* m_Layout;
};

