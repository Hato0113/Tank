//------------------------------
//	ロードシーン
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "SceneLoad.h"
#include <thread>
#include <memory>
#include "Scene\SceneManager.h"
#include "MySystem\Texture\TextureManager.h"
#include "MySystem\Model\ModelManager.h"
#include "MySystem\Sprite\SpriteManager.h"
#include "Component\Polygon\Polygon.h"
#include "Component\Loading\Loading.h"
#include "MySystem\Effect\EffectManager.h"

namespace
{
	std::unique_ptr<std::thread> TextureThread;
	std::unique_ptr<std::thread> ModelThread;
	std::unique_ptr<std::thread> SpriteThread;
	std::unique_ptr<std::thread> EffectThread;
}

SceneLoad::SceneLoad()
{
	m_LoadProcess = 0;
	m_LoadProgress = 0;
	m_CurrentCount = 0;
}

void SceneLoad::Init()
{
	TextureThread = std::make_unique<std::thread>([this]() {this->LoadTexture(); });	//テクスチャ読み込み開始
	ModelThread = std::make_unique<std::thread>([this]() {this->LoadModel(); });	//モデル読み込み開始
	SpriteThread = std::make_unique<std::thread>([this]() {this->LoadSprite(); });	//スプライトデータ読み込み開始
	EffectThread = std::make_unique<std::thread>([this]() {this->LoadEffect(); });	//エフェクトデータ読み込み

	m_LoadProcess = 4;	//スレッドの総数 

	//-- ロード画面セット --
	{
		TextureManager::Load(TextureID::BG01, "data/texture/bg.png");
		auto obj = Object::Create("LoadCover");
		auto poly = obj->AddComponent<CPolygon>();
		//poly->SetTex(TextureManager::Get(TextureID::BG01));
		poly->SetColor({ 0.0f,0.0f,0.0f });
		poly->SetSize({ static_cast<float>(WindowInfo::m_ScreenWidth),static_cast<float>(WindowInfo::m_ScreenHeight) });
		manager->Add(obj);
	}
	{
		TextureManager::Load(TextureID::Loading01, "data/texture/Load/loading01.png");
		TextureManager::Load(TextureID::Loading02, "data/texture/Load/loading02.png");
		TextureManager::Load(TextureID::Loading03, "data/texture/Load/loading03.png");
		auto obj = Object::Create("Loading");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ 512.0f,256.0f });
		poly->SetTex(TextureManager::Get(TextureID::Loading01));
		poly->SetPos({ 350.0f,-300.0f });
		obj->AddComponent<Loading>();
		manager->Add(obj);
	}
}

void SceneLoad::Update()
{
	if (m_CurrentCount++ > MinCount && m_LoadProcess <= m_LoadProgress)
		SceneManager::GetInstance().SetNextChange(SceneType::Title);

	SceneBase::Update();
}

void SceneLoad::Draw()
{
	SceneBase::Draw();
}

void SceneLoad::Uninit()
{
	TextureThread.get()->join();
	ModelThread.get()->join();
	SpriteThread.get()->join();
	EffectThread.get()->join();

	SceneBase::Uninit();
}

void SceneLoad::LoadTexture()
{
	TextureManager::Load(TextureID::NullTex, "data/texture/NullTex.jpg");
	TextureManager::Load(TextureID::Field01, "data/texture/Field01.jpg");
	TextureManager::Load(TextureID::VillageItem, "data/texture/VillageItem.png");
	TextureManager::Load(TextureID::RoadGrass, "data/texture/Road_grass.png");
	TextureManager::Load(TextureID::TitleCursor, "data/texture/Title/Cursor.png");
	TextureManager::Load(TextureID::Start, "data/texture/Title/strHajimeru.png");
	TextureManager::Load(TextureID::End, "data/texture/Title/strOwaru.png");
	TextureManager::Load(TextureID::TitleLogo, "data/texture/Title/TitleLogo.png");
	TextureManager::Load(TextureID::TargetCursor, "data/texture/Target/TargetCursor.png");
	TextureManager::Load(TextureID::TargetPoint, "data/texture/Target/TargetPoint.png");
	m_LoadProgress++;
}

void SceneLoad::LoadModel()
{
	ModelManager::Load(ModelID::Test01, "data/model/Test/Sword And Shield.fbx");
	ModelManager::Load(ModelID::Field01, "data/model/Test/cube01.fbx");
	ModelManager::Load(ModelID::Field02, "data/model/Fields/ground01.fbx");
	ModelManager::Load(ModelID::AirPlane, "data/model/airplane/airplane.obj");
	ModelManager::Load(ModelID::Bullet, "data/model/Bullet/Bullet.fbx");
	ModelManager::Load(ModelID::SandGround, "data/model/Fields/sandGround.fbx");
	ModelManager::Load(ModelID::ColorBall_Red, "data/model/ColorBall/RedBall.fbx");
	ModelManager::Load(ModelID::ColorBall_Blue, "data/model/ColorBall/BlueBall.fbx");
	ModelManager::Load(ModelID::ColorBall_Green, "data/model/ColorBall/GreenBall.fbx");
	ModelManager::Load(ModelID::ColorBall_Cyan, "data/model/ColorBall/CyanBall.fbx");
	ModelManager::Load(ModelID::ColorBall_Magenta, "data/model/ColorBall/MagentaBall.fbx");
	ModelManager::Load(ModelID::ColorBall_Yellow, "data/model/ColorBall/YellowBall.fbx");
	ModelManager::Load(ModelID::EnemyPlane, "data/model/airplane/Enemy/Enemy2.obj");
	ModelManager::Load(ModelID::LargeRock, "data/model/LargeRock/LargeRock.fbx");
	ModelManager::Load(ModelID::SkyDome, "data/model/Sky/Sky01.fbx");

	m_LoadProgress++;
}

void SceneLoad::LoadSprite()
{
	SpriteManager::Load(SpriteType::VillageItem, "data/SpriteData/VillageItem.csv", TextureID::VillageItem, 512, 512);
	m_LoadProgress++;
}

void SceneLoad::LoadEffect()
{
	EffectManager::Load(EffectID::Test01, u"data/Effect/oura.efk");
	m_LoadProgress++;
}

