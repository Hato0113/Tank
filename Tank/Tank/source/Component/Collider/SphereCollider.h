//--------------------------
//	球コライダークラス
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Collider.h"

//-- クラス定義 --
class SphereCollider : public Collider
{
public:
	SphereCollider();
	virtual ~SphereCollider() = default;

public:
	void SetRadius(float radius);
	float GetRadius() { return m_Radius; }

	//-- メンバ変数 --
private:
	float m_Radius;
	const ColliderType Type = ColliderType::Sphere;
};
