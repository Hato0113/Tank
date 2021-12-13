//-------------------------
//	コライダーマネージャー
//-- author --
//	HatoriMasashi
//-------------------------

//-- include --
#include "ColliderManager.h"

void ColliderManager::Init()
{
	Reset();
}

void ColliderManager::Update()
{
	if (m_ColliderArray.empty()) return;

	for (auto com : m_ColliderArray)
	{
		if(com->m_IsSubject)
			com->Hit();	//主体ならヒット判定を飛ばす
	}
}

void ColliderManager::Uninit()
{
	Reset();
}

/*
	リストリセット
*/
void ColliderManager::Reset()
{
	if (!m_ColliderArray.empty())
		m_ColliderArray.clear();
}

/*
	リスト追加
*/
void ColliderManager::Add(Collider* col)
{
	m_ColliderArray.push_back(col);
}

/*
	コンポーネント削除
*/
void ColliderManager::Remove(Collider* col)
{
	for (auto it = m_ColliderArray.begin(); it != m_ColliderArray.end();)
	{
		if (*it == col)
		{
			m_ColliderArray.erase(it);
			return;
		}
		else it++;
	}
}