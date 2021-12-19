//--------------------------
//	武器通常射撃
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once
//-- include --
#include "Component\Component.h"
#include "../Weapon.h"


//-- クラス定義 --
class NormalWeapon : public WeaponBase
{
public:
	NormalWeapon();
	~NormalWeapon() = default;
public:
	void Update() override;
	void SetDiray(int di) { m_DirayCount = m_Diray = di; }
	void SetBulletNum(int num) { m_MaxBullet = num; }
	void DecBullet();	//弾削除時コールバック用

private:
	int m_Diray;
	int m_DirayCount;
	int m_MaxBullet;	//最大玉数
	int m_CurrentBullet;	//現在発射玉数
};
