/**
 * @brief メッシュ用頂点シェーダー
 * @author HatoriMasashi
 */
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
	HRESULT Make(std::string) override;
};