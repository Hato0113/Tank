//--------------------------
//	ボックスコライダークラス
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "BoxCollider.h"
#include "Function\Primitive\Primitive.h"

BoxCollider::BoxCollider()
{
	const float DefaultSize = 5.0f;
	m_Size = { DefaultSize,DefaultSize,DefaultSize };
	m_Type = Type;
}

void BoxCollider::SetSize(DirectX::XMFLOAT3 size)
{
	m_Size = size;
	if (m_pMesh)
	{
		m_pMesh->SetState(false);	//削除
	}
	PrimitiveInfoCube info;
	info.m_Size = { m_Size.x,m_Size.y,m_Size.z };
	info.m_Dif = { 0.0f,1.0f,1.0f,0.4f };
	m_pMesh = Primitive::CreateCubePrimitive(parent, info);
	m_pMesh->SetDrawFlag(false);
}

void BoxCollider::SetSize(float size)
{
	SetSize({ size,size,size });
}