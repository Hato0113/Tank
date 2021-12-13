//-------------------------
//	デバッグポーズ
//-- author --
//	HatoriMasashi
//-------------------------

//-- include --
#include "DebugPause.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "Scene\SceneManager.h"

DebugPause::DebugPause()
{
	m_IsPausing = false;
}

void DebugPause::Update()
{
	bool prev = m_IsPausing;
	if (KeyInput::GetKeyPush(VK_F1))
		m_IsPausing ^= 1;

	if (prev == m_IsPausing) return;

	//-- シーン内のPause状態を変更 --
	auto manager = SceneManager::GetInstance().GetCurrentScene()->manager;
	auto list = manager->GetList();
	for (auto obj : list)
	{
		obj->SetPause(m_IsPausing);
	}
	//-- カメラは停止しない --
	auto camera = manager->FindObjectWithTag("Camera");
	camera->SetPause(false);
}