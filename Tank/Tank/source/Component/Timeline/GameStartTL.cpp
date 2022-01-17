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
#include "Component\Polygon\Polygon.h"
#include "MySystem\LevelManager\LevelManager.h"
#include "MySystem\GameManager\GameManager.h"
#include "MySystem\Texture\TextureManager.h"
#include "Component\Polygon\PolygonFade.h"
#include "System\Sound.h"
#include "Component\Enemy\EnemyAI\EnemyTaskWeapon.h"

GameStartTL::GameStartTL()
{
	//nonne
}

void GameStartTL::Init()
{
	int lifeTime = 100;
	{	//
		auto obj = Object::Create("back");
		obj->SetLifeTime(lifeTime);
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetLayer(Layer::Front2D);
		poly->SetColor({ 0.0f,0.0f,0.0f });
		poly->SetSize({ static_cast<float>(WindowInfo::m_ScreenWidth),static_cast<float>(WindowInfo::m_ScreenHeight) });
		auto pf = obj->AddComponent<PolygonFade>();
		pf->SetFade(60, 40);
		parent->GetScene()->manager->Add(obj);
	}
	{	//ステージ名表示
		auto obj = Object::Create("stageName");
		obj->SetLifeTime(lifeTime);
		FontManager::CreateString(obj, GameManager::GetLevelName(), { -350.0f,100.0f }, 4.0f, { 1.0f,1.0f,1.0f,1.0f }, true);
		auto pf = obj->AddComponent<PolygonFade>();
		pf->SetFade(60, 40);
		parent->GetScene()->manager->Add(obj);
	}
	{	//敵数表示
		auto obj = Object::Create("enemyNum");
		obj->SetLifeTime(lifeTime);
		auto EnemyList = parent->GetScene()->manager->FindObjectsWithTag("Enemy");
		int EnemyNum = static_cast<int>(EnemyList.size());
		std::string str = "x ";
		str += std::to_string(EnemyNum);
		FontManager::CreateString(obj, str, { 0.0f,-100.0f }, 2.0f, { 1.0f,1.0f,1.0f,1.0f }, true);
		auto pf = obj->AddComponent<PolygonFade>();
		pf->SetFade(60, 40);
		parent->GetScene()->manager->Add(obj);
	}
	{	//敵ポリゴン表示
		auto obj = Object::Create("enemyPoly");
		obj->SetLifeTime(lifeTime);
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetTex(TextureManager::Get(TextureID::UI_TankRed));
		poly->SetSize({ 80.0f,80.0f });
		poly->SetPos({ -100.0f,-100.0f });
		poly->SetLayer(Layer::Front2D);
		auto pf = obj->AddComponent<PolygonFade>();
		pf->SetFade(60, 40);
		parent->GetScene()->manager->Add(obj);
	}
}

void GameStartTL::Update()
{

	switch (m_CurrentTime)
	{
	case 160: {
		auto obj = Object::Create("strGame");
		obj->SetLifeTime(80);
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetTex(TextureManager::Get(TextureID::game_Game));
		poly->SetSize({ 512.0f * 1.5f,256.0f * 1.5f });
		//FontManager::CreateString(obj, "Game", { -200.0f,0.0f }, 5.0f, { 1.0f,165.0f / 255.0f,0.0f,1.0f });
		parent->GetScene()->manager->Add(obj);
	}
			break;
	case 240: {
		auto obj = Object::Create("strStart");
		obj->SetLifeTime(80);
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetTex(TextureManager::Get(TextureID::game_Start));
		poly->SetSize({ 768.0f * 1.4f,256.0f * 1.4f });
		//FontManager::CreateString(obj, "Start", { -250.0f,0.0f }, 5.0f, { 1.0f,165.0f / 255.0f,0.0f,1.0f });
		parent->GetScene()->manager->Add(obj);
	}
			break;
	case 290:
		//-- BGM --
		CSound::Play(BGM_GAME);
		
		//-- プレイヤーの操作開始 --
	{
		auto player = parent->GetScene()->manager->FindObjectWithTag("Player");
		if (player)
		{
			player->GetComponent<PlayerOperation>()->SetActive(true);
			player->GetComponent<WeaponBase>()->SetActive(true);
		}
	}

	//-- 敵行動開始 --
	{
		auto enemys = parent->GetScene()->manager->FindObjectsWithTag("Enemy");
		for (auto obj : enemys)
		{
			obj->GetComponent<WeaponBase>()->SetActive(true);
			obj->GetComponent<EnemyTaskWeapon>()->SetActive(true);
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

