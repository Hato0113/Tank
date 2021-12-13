//----------------------------------
//	メッシュ用頂点シェーダー
//-- author --
//	HatoriMasashi
//----------------------------------
#pragma once

//-- include --
#include "../VertexBase.h"

//-- クラス定義 --
class PolygonVertex : public VertexBase
{
public:
	PolygonVertex() = default;
	virtual ~PolygonVertex() = default;
public:
	HRESULT Make(std::string) override;
};

