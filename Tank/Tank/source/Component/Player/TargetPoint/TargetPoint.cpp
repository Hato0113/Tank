//--------------------------
//	ターゲットポイント
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "TargetPoint.h"
#include "MySystem\Texture\TextureManager.h"

TargetPoint::TargetPoint()
{
	m_MeshArray.clear();
	m_pWepBase = nullptr;
}

TargetPoint::~TargetPoint()
{
	//-- ポイント用オブジェクトを削除 --
	if (!m_MeshArray.empty())
		for (auto obj : m_MeshArray)
			obj->SetState(false);
}

void TargetPoint::Update()
{
	if (!m_pWepBase)
	{
		m_pWepBase = parent->GetComponent<WeaponBase>();
		if (!m_pWepBase) return;
		Create();
	}

	//-- 位置更新 --
	auto dir = m_pWepBase->GetDir();

	const float space = 10.0f;
	auto pos = parent->transform->GetPos();
	for (int i = 0; i < m_MeshArray.size(); i++)
	{
		auto target = pos;
		target.x += space * (i + 1) * dir.x;
		target.y = 20.0f;
		target.z += space * (i + 1) * dir.z;
		m_MeshArray[i]->transform->SetPos(target);
	}
}

void TargetPoint::Create()
{
	for (int i = 0; i < pointNum; i++)
	{
		auto obj = Object::Create("TargetPoint");
		PrimitiveInfoField info;
		info.m_pTex = TextureManager::Get(TextureID::TargetPoint);
		info.m_Size = { 1.5f,1.5f };
		Primitive::CreateFieldPrimitive(obj, info);
		parent->GetScene()->manager->Add(obj);
		m_MeshArray.push_back(obj);
	}
	{	//先端
		auto obj = Object::Create("TargetPoint");
		PrimitiveInfoField info;
		info.m_pTex = TextureManager::Get(TextureID::TargetCursor);
		info.m_Size = { 7.0f,7.0f };
		Primitive::CreateFieldPrimitive(obj, info);
		parent->GetScene()->manager->Add(obj);
		m_MeshArray.push_back(obj);
	}
}
