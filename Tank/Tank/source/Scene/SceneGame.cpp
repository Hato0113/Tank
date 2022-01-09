//------------------------------
//	ゲームシーンクラス
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "SceneGame.h"
#include "Objects\ObjectManager.h"
#include "Component\Camera\Camera.h"
#include "Component\Light\DirectionalLight.h"
#include "Function\Primitive\Primitive.h"
#include "Component/Polygon/Polygon.h"
#include "MySystem\Texture\TextureManager.h"
#include "Component\Model\Model.h"
#include "MySystem\Sprite\SpriteManager.h"
#include "Component\Collider\SphereCollider.h"
#include "Component\Collider\BoxCollider.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "Component\Player\PlayerOperation.h"
#include "Component\Collider\SlopeCollider.h"
#include "Component\modelAnimation\modelAnimation.h"
#include "Component\Player\Weapon\NormalWeapon.h"
#include "Component\Player\TargetPoint\TargetPoint.h"
#include "Component\Enemy\EnemyManager.h"
#include "Component\Timeline\GameStartTL.h"
#include "Component\Effect\Effect.h"
#include "MySystem\Resident\ResidentData.h"
#include "MySystem\Resident\ResidentFlag.h"
#include "MySystem\LevelManager\MapManager.h"
#include "MySystem\GameManager\GameManager.h"
#include "Component\GameUI\BallUI\BallUI.h"
#include "Component\GameUI\TankUI\EnemyTankUI.h"
#include "MySystem\EnemyAIManager\EnemyAIManager.h"

SceneGame::SceneGame()
{
	//none
}

void SceneGame::Init()
{
	//-- EnemyManager --
	EnemyManager::Reset();

	//-- カメラ --
	if (true) {
		Object* pObj = Object::Create("MainCamera");
		pObj->transform->SetTag("Camera");
		Camera* pCam = pObj->AddComponent<Camera>();
		const DirectX::XMFLOAT3 DefaultCameraPos{ 0.0f,325.0f,-200.0f };
		const DirectX::XMFLOAT3 DefaultTargetPos{ 0.0f,185.0f,-100.0f };
		pCam->SettingTransform(DefaultCameraPos, DefaultTargetPos);
		manager->Add(pObj);
	}

	//-- ライト --
	if (true) {
		Object* pLight = Object::Create("Light");
		pLight->AddComponent<DirectionalLight>();
		manager->Add(pLight);
	}

	SummonStage();	//ステージ生成

	{	//スタート時タイムライン
		auto obj = Object::Create("timeline");
		manager->Add(obj);
		obj->AddComponent<GameStartTL>();
	}

	if(true){	//スカイドーム
		auto obj = Object::Create("SkyDome");
		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::SkyDome));
		model->SetScale(500.0f);
		model->SetUseLight(false);
		Quaternion q;
		q.SetToRotateAxisAngle(Quaternion::Right, DirectX::XM_PI / 2);
		Quaternion origin;
		EulerAngles ea = obj->transform->GetEulerAngle();
		origin.SetQuaternion(ea);
		origin *= q;
		ea.SetEulerAngles(origin);
		obj->transform->SetRotate(ea);
		manager->Add(obj);
	}

	//-- 三軸 --
	if (true) {
		Object* pAxis = Object::Create("Axis");
		PrimitiveInfoLine axis;
		axis.m_startPos = { 0.0f,0.0f,0.0f };
		axis.m_endPos = { 1000.0f,0.0f,0.0f };
		axis.m_startDif = { 1.0f,0.0f,0.0f,1.0f };
		axis.m_endDif = axis.m_startDif;
		Primitive::CreateLinePrimitive(pAxis, axis);
		axis.m_startPos = { 0.0f,0.0f,0.0f };
		axis.m_endPos = { 0.0f,1000.0f,0.0f };
		axis.m_startDif = { 0.0f,1.0f,0.0f,1.0f };
		axis.m_endDif = axis.m_startDif;
		Primitive::CreateLinePrimitive(pAxis, axis);
		axis.m_startPos = { 0.0f,0.0f,0.0f };
		axis.m_endPos = { 0.0f,0.0f,1000.0f };
		axis.m_startDif = { 0.0f,0.0f,1.0f,1.0f };
		axis.m_endDif = axis.m_startDif;
		Primitive::CreateLinePrimitive(pAxis, axis);
		manager->Add(pAxis);
	}

	//-- AIマネージャ初期化 --
	EnemyAIManager::GetInstance().Init();
}

void SceneGame::Update()
{
	SceneBase::Update();

	//-- debug --
	//当たり判定の可視化 / 不可視化
	static bool state = ResidentFlagManager::GetData().SystemFlag.ShowCollider;
	if (state != ResidentFlagManager::GetData().SystemFlag.ShowCollider)
	{
		state = ResidentFlagManager::GetData().SystemFlag.ShowCollider;
		for (auto obj : manager->GetList())
		{
			Collider* pCol = obj->GetComponent<Collider>();
			if (!pCol) continue;
			else
			{
				pCol->m_pMesh->SetDrawFlag(state);
			}
		}
	}

	if (KeyInput::GetKeyPush(VK_F3))
	{
		auto obj = Object::Create("testEffect");
		obj->transform->SetPos({ 0.0f,30.0f,0.0f });
		auto effect = obj->AddComponent<Effect>();
		effect->SetEffect(EffectManager::Get(EffectID::Hit01));
		effect->SetScale(3.0f);
		obj->SetLifeTime(240);
		manager->Add(obj);
	}

	//-- 敵AI更新 --
	EnemyAIManager::GetInstance().Update();
}

void SceneGame::Draw()
{
	SceneBase::Draw();
}

void SceneGame::Uninit()
{
	SceneBase::Uninit();

	//-- 敵AIマネージャー初期化 --
	EnemyAIManager::GetInstance().Uninit();
}

void SceneGame::SummonStage()
{
	/*
		memo
		0,0,0中心
		幅 : +-150
		縦 : +140 / -60
	*/
	//床
	{
		auto obj = Object::Create("field");
		obj->transform->SetPos({ 0.0f,10.0f,50.0f });
		PrimitiveInfoField info;
		info.m_Size = { 200.0f,200.0f };
		info.m_pTex = TextureManager::Get(TextureID::FieldTex);
		Primitive::CreateFieldPrimitive(obj, info);
		manager->Add(obj);
	}
	if(false){
		DirectX::XMFLOAT3 defpos{ -140.0f,0.0f,-50.0f };
		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 15; x++)
			{
				Object* pObj = Object::Create("TestField");
				pObj->transform->SetPos({ defpos.x + 20.0f * x,defpos.y,defpos.z + 20.0f * y });
				auto model = pObj->AddComponent<Model>();
				model->SetModel(ModelManager::Get(ModelID::Field02));
				model->SetScale(10.0f);
				Quaternion q;
				q.Identity();
				q.SetToRotateAxisAngle(Quaternion::Right, DirectX::XM_PI / 2);
				EulerAngles ea;
				ea.SetEulerAngles(q);
				pObj->transform->SetRotate(ea);
				manager->Add(pObj);
			}
		}
	}
	//-- 壁 --
	{
		//-- ステージ作成テンプレ --
		auto make = [](DirectX::XMFLOAT3 pos, ObjectManager* manager)
		{
			const float WallColor[] =
			{
				1.0f,0.8f,0.6f,0.4f
			};
			const int ColorMax = _countof(WallColor);
			const int Color = rand() % ColorMax;

			auto obj = Object::Create("wall");
			obj->transform->SetPos({ pos.x,pos.y,pos.z });
			obj->transform->SetTag("Field");
			auto model = obj->AddComponent<Model>();
			model->SetModel(ModelManager::Get(ModelID::WoodBlock));
			model->SetScale(10.0f);
			model->SetDiffuse({ WallColor[Color],WallColor[Color],WallColor[Color],1.0f });
			Quaternion q;
			q.Identity();
			q.SetToRotateAxisAngle(Quaternion::Right, DirectX::XM_PI / 2);
			EulerAngles ea;
			ea.SetEulerAngles(q);
			obj->transform->SetRotate(ea);

			manager->Add(obj);
		};
		const int width = 17;
		const int height = 10;
		const float defY = 0.0f;
		const float fieldSize = 20.0f;
		for (int z = 1; z < 2; z++)	//段数
		{
			for (int y = 0; y < height; y++)
			{
				make({ -160.0f,defY + fieldSize * z,-50.0f + fieldSize * y }, manager);
				make({ 160.0f,defY + fieldSize * z,-50.0f + fieldSize * y }, manager);
			}
			for (int x = 0; x < width; x++)
			{
				make({ -160.0f + fieldSize * x,defY + fieldSize * z,-78.0f }, manager);
				make({ -160.0f + fieldSize * x,defY + fieldSize * z,150.0f }, manager);
			}
		}

		//-- コライダー --
		{
			struct
			{
				const char* name;
				DirectX::XMFLOAT3 pos;
				DirectX::XMFLOAT3 size;
			}status[4] =
			{
				{"ZWall",{0.0f,20.0f,-70.0f},{150.0f,10.0f,10.0f}},
				{"ZWall",{0.0f,20.0f,150.0f},{150.0f,10.0f,10.0f}},
				{"XWall",{-160.0f,20.0f,40.0f},{10.0f,10.0f,150.0f}},
				{"XWall",{160.0f,20.0f,40.0f},{10.0f,10.0f,150.0f}},
			};
			for (int i = 0; i < 4; i++)
			{
				auto obj = Object::Create(status[i].name);
				obj->transform->SetPos(status[i].pos);
				obj->transform->SetTag("Field");
				auto col = obj->AddComponent<BoxCollider>();
				col->SetSize(status[i].size);
				manager->Add(obj);
			}
			

		}
	}
	//-- オブジェクト生成 --
	GameManager::CreateLevel();
	/*
	//-- プレイヤー召喚 --
	{
		Object* pObj = Object::Create("Player");
		pObj->transform->SetPos({ 0.0f,14.0f,0.0f });
		pObj->transform->SetTag("Player");
		auto model = pObj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::Player_Body));
		model->SetScale(5.0f);
		model->SetRelativePos({ 0.0f,-2.0f,0.0f });
		auto head = pObj->AddComponent<Model>();
		head->SetModel(ModelManager::Get(ModelID::Player_Head));
		head->SetScale(6.0f);
		head->SetRelativePos({ 0.0f,4.0f,0.0f });
		head->SetUseParentRotate(false);
		auto op = pObj->AddComponent<PlayerOperation>();
		op->SetActive(false);
		auto weapon = pObj->AddComponent<NormalWeapon>();
		weapon->SetWeaponMode(WeaponMode::OpSelf);
		weapon->SetActive(false);
		weapon->SetDiray(ResidentDataManager::GetData().PlayerData.ShotRate);
		weapon->SetTankHead(head);
		auto target = pObj->AddComponent<TargetPoint>();
		auto col = pObj->AddComponent<SphereCollider>();
		col->SetRadius(5.0f);
		col->SetSubjectState(true);
		manager->Add(pObj);
	}
	{	//敵召喚
		//x : 0 ~ 14 y : 0 ~ 9
		DirectX::XMINT2 poss[] =
		{
			{1,1},{5,5},{14,5}
		};
		for (int i = 0; i < _countof(poss); i++)
		{
			EnemyManager::Summon(poss[i], EnemyType::Normal);
		}
	}
	{
		manager->Add(MapManager::GetInstance().CreateMapObject({ 2,2 }, PanelType::Wall));
		manager->Add(MapManager::GetInstance().CreateMapObject({ 2,3 }, PanelType::Wall));
		manager->Add(MapManager::GetInstance().CreateMapObject({ 10,6 }, PanelType::Wall));
		manager->Add(MapManager::GetInstance().CreateMapObject({ 11,6 }, PanelType::Wall));
		manager->Add(MapManager::GetInstance().CreateMapObject({ 12,6 }, PanelType::Wall));
	}
	*/

	//-- UI生成 --
	{
		auto obj = Object::Create("ballUI");
		obj->AddComponent<BallUI>();
		manager->Add(obj);
	}
	{
		auto obj = Object::Create("enemyUI");
		obj->AddComponent<EnemyTankUI>();
		manager->Add(obj);
	}
	{	//UI帯
		auto obj = Object::Create("ui_Band");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetPos({ 0.0f,-315.0f });
		poly->SetSize({ static_cast<float>(WindowInfo::m_ScreenWidth),90.0f });
		poly->SetColor({ 0.2f,0.2f,0.2f });
		poly->SetAlpha(0.6f);
		poly->SetLayer(Layer::Back2D);
		manager->Add(obj);
	}
}
