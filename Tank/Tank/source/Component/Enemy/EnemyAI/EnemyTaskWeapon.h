//---------------------
//	敵攻撃AI
//-- author --
//	HatoriMasashi
//---------------------
#pragma once

//-- include --
#include "EnemyTaskBase.h"
#include "Component\Player\Weapon.h"

/*
	攻撃タスク
	タスク時間を2Fにしておき、そのたびに射撃可能か判定
*/

//-- クラス定義 --
class EnemyTaskWeapon : public EnemyTaskBase
{
public:
	EnemyTaskWeapon();
	virtual ~EnemyTaskWeapon() = default;

	void Update() override;

	bool GetCanShot() { return m_canShot; }
	void SetCanShot(bool state) { m_canShot = state; }
	void SetWeapon(WeaponBase* wep) { m_Weapon = wep; }

private:
	bool m_canShot;
	WeaponBase* m_Weapon;
};
