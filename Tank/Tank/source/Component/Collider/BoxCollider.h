//--------------------------
//	ボックスコライダークラス
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Collider.h"
#include <DirectXMath.h>

//-- クラス定義 --
class BoxCollider : public Collider
{
public:
	BoxCollider();
	virtual ~BoxCollider() = default;

public:
	void SetSize(DirectX::XMFLOAT3 size);
	void SetSize(float size);
	DirectX::XMFLOAT3 GetSize() { return m_Size; }

private:
	DirectX::XMFLOAT3 m_Size;	//四角形のハーフサイズ
	const ColliderType Type = ColliderType::Box;
};
