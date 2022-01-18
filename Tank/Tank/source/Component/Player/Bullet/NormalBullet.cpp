//--------------------------
//	通常弾
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "NormalBullet.h"
#include "IMGUI\GUI_Message.h"
#include "MySystem\Effect\EffectManager.h"
#include "Component\Effect\Effect.h"

namespace
{
	const float Speed = 1.0f;
	const int MaxHit = 2;
}

NormalBullet::NormalBullet()
{
	m_Speed = Speed;
	m_HitCount = 0;
}

void NormalBullet::Update()
{
	auto pos = parent->transform->GetPos();
	pos.x += Speed * m_Dir.x;
	pos.z += Speed * m_Dir.z;
	parent->transform->SetPos(pos);

	BulletBase::RotateModel(DirectX::XM_PI / 180.0f * 3.0f);
	BulletBase::Update();
}

void NormalBullet::OnCollisionEnter(Collider* other)
{
	bool hit = false;
	if (other->parent->transform->GetTag() == "Field")
	{
		//向きの変更
		auto name = other->parent->transform->GetName();
		if (other->parent->transform->GetName() == "XWall")
		{
			m_Dir.x *= -1;
		}
		else
		{
			m_Dir.z *= -1;
		}
		hit = true;
	}
	if (other->parent->transform->GetTag() == "MapObject")
	{
		DirectX::XMFLOAT3 dif;
		auto otherPos = other->parent->transform->GetPos();
		auto myPos = parent->transform->GetPos();
		dif.x = otherPos.x - myPos.x;
		dif.z = otherPos.z - myPos.z;
		if (fabsf(dif.x) > fabsf(dif.z))
		{
			m_Dir.x *= -1;
		}
		else
		{
			m_Dir.z *= -1;
		}
		hit = true;
	}

	//-- ヒット回数カウント --
	if (hit)
	{
		m_HitCount++;
		if (m_HitCount >= MaxHit)
			parent->SetEnable(false);	//オブジェクト破棄

		auto obj = Object::Create("testEffect");
		obj->transform->SetPos(parent->transform->GetPos());
		auto effect = obj->AddComponent<Effect>();
		effect->SetEffect(EffectManager::Get(EffectID::HitEffect));
		effect->SetScale(2.0f);
		obj->SetLifeTime(120);
		parent->GetScene()->manager->Add(obj);
	}

	BulletBase::OnCollisionEnter(other);
}
