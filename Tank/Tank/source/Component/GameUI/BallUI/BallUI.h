//------------------------------
//	ボールUI
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Player\Weapon\NormalWeapon.h"
#include "Component\Polygon\Polygon.h"

//-- クラス定義 --
class BallUI : public Component
{
public:
	BallUI();
	virtual ~BallUI() = default;

	void Init() override;
	void Update() override;

private:
	std::vector<CPolygon*> m_Balls;
	NormalWeapon* m_PlayerWeapon;
};
