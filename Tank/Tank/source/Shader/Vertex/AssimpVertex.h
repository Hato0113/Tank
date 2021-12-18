//-----------------------------
//	Assimp用頂点シェーダー
//-- author --
//	HatoriMasashi
//-----------------------------
#pragma once

//-- include --
#include "Shader\VertexBase.h"

//-- クラス定義 --
class AssimpVertex : public VertexBase
{
public:
	AssimpVertex() = default;
	virtual ~AssimpVertex() = default;
public:
	/*
		シェーダー作成関数
		引数 : ファイル名
		戻り値 : 結果
	*/
	HRESULT Make(std::string) override;
};
