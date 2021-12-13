//------------------------------
//	シーンマネージャークラス
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "SceneManager.h"
#include "Allocator/Allocator.hpp"
#include "ConstantScene.h"

SceneManager::SceneManager()
{
	m_pCurrentScene = nullptr;
	m_pNextScene = nullptr;
	m_Fade = nullptr;
}

void SceneManager::Update()
{
	//-- シーン変化 --
	if (m_pNextScene)
	{
		if (m_Fade->GetEventTrigger() == FadeEvent::Middle)
		{
			if (m_pCurrentScene)
			{
				m_pCurrentScene->Uninit();
			}
			m_pCurrentScene = m_pNextScene;
			//-- オブジェクトマネージャー生成 --
			m_pCurrentScene->manager = Allocator::Alloc<ObjectManager>();
			m_pCurrentScene->manager->m_pScene = m_pCurrentScene;
			m_pCurrentScene->Init();	//シーン初期化
			m_Fade->SetUp(FadeMode::FadeOut);
		}
		if(m_Fade->GetEventTrigger() == FadeEvent::End)
			m_pNextScene = nullptr;
	}

	//-- シーン更新 --
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->Update();
	}
}

void SceneManager::Draw()
{
	if (m_pCurrentScene)
		m_pCurrentScene->Draw();
}

void SceneManager::Add(SceneType type, SceneBase* pScene)
{
	m_SceneMap[type] = pScene;
}

void SceneManager::SetNextChange(SceneType type)
{
	if (m_pNextScene == nullptr && m_SceneMap.find(type) != m_SceneMap.end())
	{
		m_pNextScene = m_SceneMap[type];
		m_CurrentType = type;
		//-- フェード開始 --
		if (!m_Fade)
		{
			auto obj = ConstantScene::GetInstance().manager->FindObjectWithName("FadeCover");
			m_Fade = obj->GetComponent<Fade>();
		}
		m_Fade->SetUp(FadeMode::FadeIn);
	}
	else return;
}

void SceneManager::SetFirst(SceneType type)
{
	m_pCurrentScene = m_SceneMap[type];
	m_CurrentType = type;
	//-- オブジェクトマネージャー生成 --
	m_pCurrentScene->manager = Allocator::Alloc<ObjectManager>();
	m_pCurrentScene->manager->m_pScene = m_pCurrentScene;
	m_pCurrentScene->Init();	//シーン初期化
}

void SceneManager::Uninit()
{
	if (m_pCurrentScene)
		m_pCurrentScene->Uninit();

	//-- シーン開放 --
	for (auto it : m_SceneMap)
	{
		Allocator::Free(it.second);
	}
	m_SceneMap.clear();
}

