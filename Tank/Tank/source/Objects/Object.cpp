//--------------------------
//	オブジェクトクラス
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Object.h"
#include "Component\Transform.h"

Object::Object()
{
	transform = nullptr;
	m_Scene = nullptr;
	m_ComponentList.clear();
	m_Enable = true;
	m_Pause = false;
	m_LifeTime = -1;
	m_Visible = true;
	m_Active = true;
}

Object::~Object()
{
	//--　コンポーネント全解放 --
	for (auto com : m_ComponentList)
	{
		Allocator::Free(com);
	}

	m_ComponentList.clear();
}

void Object::Update()
{
	for (auto it = m_ComponentList.begin(); it != m_ComponentList.end();)
	{
		if ((*it)->GetState())
		{
			if((*it)->GetActive())
				(*it)->Update();
			it++;
		}
		else
		{
			Component* pCom = (*it);
			Allocator::Free(pCom);
			it = m_ComponentList.erase(it);
		}
	}

	//-- 生存時間更新 --
	if (m_LifeTime > 0)
	{
		m_LifeTime--;
		if (!m_LifeTime)
			m_Enable = false;
	}
}

void Object::Draw(Layer layer)
{
	for (auto com : m_ComponentList)
	{
		if (com->GetLayer() == layer)
		{
			com->Draw();	//指定レイヤーのみ描画
		}
	}
}

/*
	オブジェクト生成
	##この時点では所属レイヤーはわからない
*/
Object* Object::Create(std::string name)
{
	Object* ret = Allocator::Alloc<Object>();
	ret->transform = ret->AddComponent<Transform>(name);
	return ret;
}

/*
	判定イベント
*/
void Object::CollisionEnterEvent(Collider* other) const
{
	for (auto com : m_ComponentList)
	{
		com->OnCollisionEnter(other);
	}
}
void Object::CollisionStayEvent(Collider* other) const
{
	for (auto com : m_ComponentList)
	{
		com->OnCollisionStay(other);
	}
}
void Object::CollisionExitEvent(Collider* other) const
{
	for (auto com : m_ComponentList)
	{
		com->OnCollisionExit(other);
	}
}
