//--------------------------
//	武器通常射撃
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "NormalWeapon.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "Component\Player\Bullet\BulletManager.h"
#include "Component\Collider\SphereCollider.h"
#include "Component\Player\PlayerOperation.h"
#include "MySystem\Resident\ResidentData.h"
#include "MySystem\Resident\ResidentFlag.h"

NormalWeapon::NormalWeapon()
{
	WeaponBase::SetBulletType(BulletType::Normal);
	m_DirayCount = 0;
	m_CurrentBullet = 0;
	m_MaxBullet = ResidentDataManager::GetData().PlayerData.ShotMax;
}

void NormalWeapon::Update()
{
	auto pos = parent->transform->GetPos();
	auto dir = WeaponBase::m_Dir;

	switch (m_Mode)
	{
	case WeaponMode::None:
		break;
	case WeaponMode::OpSelf:
		if (m_DirayCount) m_DirayCount--;
		if ((KeyInput::GetKeyPush(VK_LBUTTON) || ResidentFlagManager::GetData().GamePlay.Player.AutoShot) &&
			(m_MaxBullet > m_CurrentBullet || ResidentFlagManager::GetData().GamePlay.Player.InfiniteBullet))
		{
			if (!m_DirayCount)
			{
				auto col = parent->GetComponent<SphereCollider>();
				float radius = col->GetRadius();
				constexpr float margin = 0.5f;
				pos.x += radius * dir.x * 2 + margin;
				pos.z += radius * dir.z * 2 + margin;
				m_CurrentBullet++;
				BulletManager::Summon(pos, dir, BulletType::Normal, ModelID::ColorBall_Cyan, this);
				m_DirayCount = m_Diray;
			}
		}
		//WeaponBase::UpdateDirSelf();
		WeaponBase::UpdateDirByMouse();
		break;
	case WeaponMode::Auto:
		if (m_DirayCount) m_DirayCount--;
		if (!m_DirayCount)
		{
			auto col = parent->GetComponent<SphereCollider>();
			float radius = col->GetRadius();
			constexpr float margin = 0.5f;
			pos.x += radius * dir.x * 2 + margin;
			pos.z += radius * dir.z * 2 + margin;
			BulletManager::Summon(pos, dir, BulletType::Normal,ModelID::ColorBall_Red);
			m_DirayCount = m_Diray;
		}
		WeaponBase::TargetPlayer();
		break;
	default:
		break;
	}
}

void NormalWeapon::DecBullet()
{
	if (!this) return;
	if (m_CurrentBullet)
		m_CurrentBullet--;
}