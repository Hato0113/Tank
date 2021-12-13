//--------------------------
//	通常弾
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "NormalBullet.h"
#include "IMGUI\GUI_Message.h"

namespace
{
	const float Speed = 0.8f;
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
		m_HitCount++;
		if (m_HitCount >= MaxHit)
			parent->SetState(false);	//オブジェクト破棄
	}

	BulletBase::OnCollisionEnter(other);
}
