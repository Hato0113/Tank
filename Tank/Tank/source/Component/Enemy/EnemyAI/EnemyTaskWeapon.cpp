//---------------------
//	敵攻撃AI
//-- author --
//	HatoriMasashi
//---------------------

//-- include --
#include "EnemyTaskWeapon.h"
#include "IMGUI\GUI_Message.h"

/*
	コンストラクタ
*/
EnemyTaskWeapon::EnemyTaskWeapon()
{
	m_End = false;
	m_RemainTime = 2;
	TaskType = "Weapon";
	m_canShot = false;
}

/*
	更新処理
*/
void EnemyTaskWeapon::Update()
{
	if(m_RemainTime) m_RemainTime--;
	if (!m_RemainTime)
	{
		m_End = true;
	}
	else
	{
		m_End = false;
	}

	//-- 打てるか否か反映 --
	if (m_Weapon)
	{
		m_Weapon->SetActive(m_canShot);
	}
}