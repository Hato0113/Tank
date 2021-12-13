//--------------------------
//	弾ベースクラス
//-- author --
//	HatoriMasashi
//--------------------------

#include "BulletBase.h"
#include "Scene\SceneManager.h"
#include "Component\Enemy\EnemyManager.h"
#include "Component\Player\Weapon\NormalWeapon.h"

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
	return;
	if (m_InvalidTime) return;

	auto tag = other->parent->transform->GetTag();
	if (tag == "Player" || tag == "Enemy" || tag == "Bullet")
	//if (tag == "Enemy" || tag == "Bullet")
	{
		parent->SetState(false);
		other->parent->SetState(false);
		if (tag == "Player")
		{
			SceneManager::GetInstance().SetNextChange(SceneType::Title);
		}
		else if (tag == "Enemy")
		{
			EnemyManager::Kill(1);
		}
		else if (tag == "Bullet")
		{

		}
	}
}
