/**
 * @brief Assimp用頂点シェーダー
 * @author HatoriMasashi
 */
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
	HRESULT Make(std::string) override;
};
