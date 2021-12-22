//--------------------------
//	オブジェクト管理
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	m_pScene = nullptr;
	m_ObjectList.clear();
}

ObjectManager::~ObjectManager()
{
	for (auto obj : m_ObjectList)
	{
		Allocator::Free(obj);
	}
	m_ObjectList.clear();
}

void ObjectManager::Update()
{
	//-- 要素数に余裕を持たせる --
	if (m_ObjectList.capacity() / 2 < m_ObjectList.size())
		m_ObjectList.reserve(m_ObjectList.capacity() * 2);

	for (auto it = m_ObjectList.begin(); it != m_ObjectList.end();)
	{
		if ((*it)->GetState())
		{
			if (!(*it)->GetPause() && (*it)->GetActive())
				(*it)->Update();
			it++;
		}
		else
		{
			Object* pObj = (*it);
			Allocator::Free(pObj);
			it = m_ObjectList.erase(it);
		}
	}
}

void ObjectManager::Draw()
{
	for (int i = static_cast<int>(Layer::Back3D); i < static_cast<int>(Layer::LayerMax); i++)
	{
		//-- Zバッファ設定 --
		switch (static_cast<Layer>(i))
		{
		case Layer::Back3D:
			DX::DirectXManager::GetInstance().SetZBuffer(true);	//通常状態
			break;
		case Layer::Front3D:
			DX::DirectXManager::GetInstance().SetZWrite(false);	//Zバッファ書き込み停止
			break;
		case Layer::Back2D:
		case Layer::Front2D:
			DX::DirectXManager::GetInstance().SetZBuffer(false);	//Zバッファ使用停止
			break;
		default:
			break;
		}

		//-- オブジェクト描画 --
		for (auto obj : m_ObjectList)
		{
			if (!obj->GetVisible()) continue;
			obj->Draw(static_cast<Layer>(i));	//全レイヤーを順番に表示
		}
	}
}

void ObjectManager::Add(Object* pObj)
{
	pObj->SetScene(m_pScene);	//シーン情報付与
	m_ObjectList.push_back(pObj);
}

std::vector<Object*> ObjectManager::FindObjectsWithName(std::string name)
{
	std::vector<Object*> ret;
	for (auto obj : m_ObjectList)
	{
		if (obj->transform->GetName() == name)
		{
			ret.push_back(obj);
		}
	}
	return ret;
}
std::vector<Object*> ObjectManager::FindObjectsWithTag(std::string tag)
{
	std::vector<Object*> ret;
	for (auto obj : m_ObjectList)
	{
		if (obj->transform->GetTag() == tag)
		{
			ret.push_back(obj);
		}
	}
	return ret;
}
Object* ObjectManager::FindObjectWithName(std::string name)
{
	for (auto obj : m_ObjectList)
	{
		if (obj->transform->GetName() == name)
			return obj;
	}
	return nullptr;
}
Object* ObjectManager::FindObjectWithTag(std::string tag)
{
	for (auto obj : m_ObjectList)
	{
		if (obj->transform->GetTag() == tag)
			return obj;
	}
	return nullptr;
}