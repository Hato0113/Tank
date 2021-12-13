//--------------------------
//	ターゲットポイント
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Function\Primitive\Primitive.h"
#include "Component\Player\Weapon.h"

//-- クラス定義 --
class TargetPoint : public Component
{
public:
	TargetPoint();
	virtual ~TargetPoint();

public:
	void Update() override;
	void Create();
public:
	const int pointNum = 6;
	const float TargetRadius = 5.0f;
	std::vector<Object*> m_MeshArray;
	WeaponBase* m_pWepBase;
};

