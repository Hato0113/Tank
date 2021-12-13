//--------------------------
//	球コライダークラス
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "SphereCollider.h"
#include "Function\Primitive\Primitive.h"

SphereCollider::SphereCollider()
{
	m_Radius = 5.0f;
	m_Type = Type;
}

void SphereCollider::SetRadius(float radius)
{
	m_Radius = radius;
	if (m_pMesh)
	{
		m_pMesh->SetState(false);
	}
	PrimitiveInfoSphere info;
	info.m_Dif = { 1.0f,0.0f,1.0f,0.4f };
	info.m_fRadius = m_Radius;
	info.m_nSlice = 20;
	info.m_nStack = 20;
	m_pMesh = Primitive::CreateSpherePrimitive(parent, info);
	m_pMesh->SetDrawFlag(false);
}
