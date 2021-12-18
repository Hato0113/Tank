//-------------------------
//	デバッグポーズ
//-- author --
//	HatoriMasashi
//-------------------------

//-- include --
#include "DebugPause.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "Scene\SceneManager.h"
#include "MySystem\Resident\ResidentFlag.h"

DebugPause::DebugPause()
{
	m_IsPausing = false;
}

void DebugPause::Update()
{
	static bool prev = ResidentFlagManager::GetData().SystemFlag.Pause;

	if (prev == ResidentFlagManager::GetData().SystemFlag.Pause) return;
	else
	{
		prev = ResidentFlagManager::GetData().SystemFlag.Pause;
	}

	//-- シーン内のPause状態を変更 --
	auto manager = SceneManager::GetInstance().GetCurrentScene()->manager;
	auto list = manager->GetList();
	for (auto obj : list)
	{
		obj->SetPause(prev);
	}
	//-- カメラは停止しない --
	auto camera = manager->FindObjectsWithTag("Camera");
	for (auto obj : camera)
		obj->SetPause(false);
}