//--------------------------
//	エフェクトコンポーネント
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Effect.h"

Effect::Effect()
{
	m_AutoPlay = true;
	m_CurrentTime = 0;
	m_MaxTime = 120;
}

void Effect::Init()
{
	m_Manager = EffectManager::m_Manager;
	auto pos = parent->transform->GetPos();
	m_Handle = m_Manager->Play(m_Effect, Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

void Effect::SetEffect(Effekseer::EffectRef effect)
{
	m_Effect = effect;
	auto pos = parent->transform->GetPos();
	m_Handle = m_Manager->Play(m_Effect, Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

void Effect::SetScale(DirectX::XMFLOAT3 scale)
{
	m_Manager->SetScale(m_Handle, scale.x, scale.y, scale.z);
}