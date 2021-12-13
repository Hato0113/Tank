//--------------------------
//	コライダーベースクラス
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include <iterator>
#include "MySystem\Collider\ColliderManager.h"

Collider::Collider()
{
	m_IsSubject = false;
	m_pMesh = nullptr;
	m_PrevHitArray.clear();

	//-- 自身をコライダーマネージャーに登録 --
	ColliderManager::GetInstance().Add(this);
}

Collider::~Collider()
{
	ColliderManager::GetInstance().Remove(this);	//登録解除
}

void Collider::Hit()
{
	if (!m_IsSubject) return;	//主体で無ければスキップ

	std::vector<Collider*> CurrentHitArray;	//現在フレームで当たった

	auto objectList = parent->GetScene()->manager->GetList();
	for (auto obj : objectList)
	{
		Collider* other = obj->GetComponent<Collider>();
		if (!other) continue;	//コライダーを持たない
		if (obj == parent) continue;	//自分自身

		bool result = false;
		switch (m_Type)
		{
		case ColliderType::Box:
			result = Collider::AABB(this, other);
			break;
		case ColliderType::Sphere:
			if (other->m_Type == ColliderType::Box)
				result = Collider::AABB(this, other);
			else
				result = Collider::Sphere(this, other);	//両方球の場合のみ球同士の判定
			break;
		default:
			break;
		}

		if (result)
		{
			CurrentHitArray.push_back(other);
		}
	}
	
	if (CurrentHitArray.empty() && m_PrevHitArray.empty()) return;

	//-- イベント発行 --
	auto check = [&](std::vector<Collider*> Array, Collider* element) {
		if (Array.empty()) return false;
		if (!element) return false;
		return (element == *std::find(Array.begin(), Array.end(), element)) ? true : false;
	};
	for (auto com : CurrentHitArray)
	{
		if (check(m_PrevHitArray, com))
		{
			//-- 当たり中 --
			parent->CollisionStayEvent(com);	//相手の情報
		}
		else
		{
			//-- 当たりはじめ --
			parent->CollisionEnterEvent(com);
		}
	}
	for (auto com : m_PrevHitArray)	//前回当たっていなかったオブジェクトとの判定
	{ 
		if (!check(CurrentHitArray, com))
		{
			//--　当たり終わり --
			parent->CollisionExitEvent(com);
		}
	}
	
	//-- リスト更新 --
	if (CurrentHitArray.empty())
		m_PrevHitArray.clear();
	else
		std::copy(CurrentHitArray.begin(), CurrentHitArray.end(), std::back_inserter(m_PrevHitArray));
}

bool Collider::AABB(Collider* my, Collider* other)
{
	DirectX::XMFLOAT3 mySize;
	DirectX::XMFLOAT3 otherSize;
	SphereCollider* SpherePtr = nullptr;
	if (SpherePtr = dynamic_cast<SphereCollider*>(my))
	{
		//-- 自身が球 --
		float size = SpherePtr->GetRadius();
		mySize = { size,size,size };
		//-- 相手はボックス --
		BoxCollider* bc = dynamic_cast<BoxCollider*>(other);
		otherSize = bc->GetSize();
	}
	else if (SpherePtr = dynamic_cast<SphereCollider*>(other))
	{
		//-- 相手が球 --
		float size = SpherePtr->GetRadius();
		otherSize = { size,size,size };
		//-- 自身はボックス --
		BoxCollider* bc = dynamic_cast<BoxCollider*>(my);
		mySize = bc->GetSize();
	}
	else
	{
		//-- 両方ボックス --
		BoxCollider* bc = dynamic_cast<BoxCollider*>(my);
		mySize = bc->GetSize();
		bc = dynamic_cast<BoxCollider*>(other);
		otherSize = bc->GetSize();
	}

	DirectX::XMFLOAT3 myPos = my->parent->transform->GetPos();
	DirectX::XMFLOAT3 otherPos = other->parent->transform->GetPos();

	//-- 高さのみ判定 --
	if (myPos.y + mySize.y >= otherPos.y - otherSize.y &&
		myPos.y - mySize.y <= otherPos.y + otherSize.y)
	{
		//-- X 軸判定 --
		if (myPos.x + mySize.x >= otherPos.x - otherSize.x &&
			myPos.x - mySize.x <= otherPos.x + otherSize.x)
		{
			//-- Z軸判定 --
			if (myPos.z + mySize.z >= otherPos.z - otherSize.z &&
				myPos.z - mySize.z <= otherPos.z + otherSize.z)
			{
				return true;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}

bool Collider::Sphere(Collider* my, Collider* other)
{
	float myRadius = 0.0f;
	float otherRadius = 0.0f;
	//-- 両方Sphere --
	SphereCollider* sph = dynamic_cast<SphereCollider*>(my);
	myRadius = sph->GetRadius();
	sph = dynamic_cast<SphereCollider*>(other);
	otherRadius = sph->GetRadius();

	DirectX::XMFLOAT3 myPos = my->parent->transform->GetPos();
	DirectX::XMFLOAT3 otherPos = other->parent->transform->GetPos();

	float xDif = myPos.x - otherPos.x;
	float yDif = myPos.y - otherPos.y;
	float zDif = myPos.z - otherPos.z;
	if (xDif * xDif + yDif * yDif + zDif * zDif <= powf(myRadius + otherRadius, 2))
	{	
		return true;
	}
	else return false;
}

