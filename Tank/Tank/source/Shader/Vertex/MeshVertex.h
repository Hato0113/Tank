//---------------------------
//	メッシュ用頂点シェーダー
//-- author --
//	HatoriMasashi
//---------------------------
#pragma once

//-- include --
#include "../VertexBase.h"

//-- クラス定義 --
class MeshVertex : public VertexBase
{
public:
	MeshVertex() = default;
	virtual ~MeshVertex() = default;
public:
	/*
		シェーダー作成
		引数 : ファイル名
		戻り値 : 結果
	*/
	HRESULT Make(std::string) override;
};