//--------------------------
//	弾ベースクラス
//-- author --
//	HatoriMasashi
//--------------------------

#include "BulletBase.h"
#include "Scene\SceneManager.h"
#include "Component\Enemy\EnemyManager.h"
#include "Component\Player\Weapon\NormalWeapon.h"
#include "MySystem\Resident\ResidentFlag.h"
#include "MySystem\Effect\EffectManager.h"
#include "Component\Effect\Effect.h"
#include "Component\Timeline\GameEndTL.h"

BulletBase::~BulletBase()
{
	m_ParentWeapon->DecBullet();
}

void BulletBase::RotateModel(float spd)
{
	Quaternion q;
	q.SetQuaternion(parent->transform->GetEulerAngle());
	Quaternion add;
	add.SetToRotateAxisAngle(Quaternion::Up, spd);
	q *= add;
	add.SetToRotateAxisAngle(Quaternion::Right, spd);
	q *= add;
	EulerAngles ea;
	ea.SetEulerAngles(q);
	parent->transform->SetRotate(ea);
}

void BulletBase::Update()
{
	if (m_InvalidTime) m_InvalidTime--;
}

void BulletBase::OnCollisionEnter(Collider* other)
{
	if (m_InvalidTime) return;

	auto tag = other->parent->transform->GetTag();
	if (tag == "Player" || tag == "Enemy" || tag == "Bullet")
	{
		//-- 無敵中 --
		if (ResidentFlagManager::GetData().GamePlay.Player.Invincible && tag == "Player")
		{
			parent->SetState(false);
			return;
		}
		
		//-- 通常処理 --
		parent->SetState(false);
		other->parent->SetState(false);

		//-- エフェクト生成 --
		auto obj = Object::Create("testEffect");
		obj->transform->SetPos(parent->transform->GetPos());
		auto effect = obj->AddComponent<Effect>();
		effect->SetEffect(EffectManager::Get(EffectID::Explosion));
		effect->SetScale(2.0f);
		obj->SetLifeTime(120);
		parent->GetScene()->manager->Add(obj);

		if (tag == "Player")
		{
			//-- プレイヤー死亡 --
			auto obj = Object::Create("EndTL");
			obj->AddComponent<GameEndTL>(false);
			parent->GetScene()->manager->Add(obj);
		}
		else if (tag == "Enemy")
		{
			if (EnemyManager::Kill(1))
			{
				//-- 敵全滅 --
				auto obj = Object::Create("EndTL");
				obj->AddComponent<GameEndTL>(true);
				parent->GetScene()->manager->Add(obj);
			}
		}
		else if (tag == "Bullet")
		{

		}
	}
}
