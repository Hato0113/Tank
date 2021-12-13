//---------------------------
//	ゲームスタート時タイムライン
//-- author --
// HatoriMasashi
//---------------------------

//-- include --
#include "GameStartTL.h"
#include "MySystem\FontManager\FontManager.h"
#include "Component\Player\PlayerOperation.h"
#include "Component\Player\Weapon.h"
#include "Component\Enemy\EnemyMovement.h"

GameStartTL::GameStartTL()
{
	//nonne
}

void GameStartTL::Update()
{
	
	switch (m_CurrentTime)
	{
	case 60: {
		auto obj = Object::Create("strGame");
		obj->SetLifeTime(80);
		FontManager::CreateString(obj, "Game", { -200.0f,0.0f }, 5.0f, { 1.0f,165.0f / 255.0f,0.0f,1.0f });
		parent->GetScene()->manager->Add(obj);
	}
		   break;
	case 140: {
		auto obj = Object::Create("strStart");
		obj->SetLifeTime(80);
		FontManager::CreateString(obj, "Start", { -250.0f,0.0f }, 5.0f, { 1.0f,165.0f / 255.0f,0.0f,1.0f });
		parent->GetScene()->manager->Add(obj);
	}
		   break;
	case 150:
		//-- プレイヤーの操作開始 --
	{
		auto player = parent->GetScene()->manager->FindObjectWithTag("Player");
		player->GetComponent<PlayerOperation>()->SetActive(true);
		player->GetComponent<WeaponBase>()->SetActive(true);
	}

		//-- 敵行動開始 --
	{
		auto enemys = parent->GetScene()->manager->FindObjectsWithTag("Enemy");
		for (auto obj : enemys)
		{
			obj->GetComponent<WeaponBase>()->SetActive(true);
			obj->GetComponent<EnemyMovement>()->SetActive(true);
		}
	}


		parent->SetState(false);
		break;
	default:
		break;
	}

	TimelineBase::m_CurrentTime++;
}

