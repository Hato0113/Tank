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
#include "IMGUI\GUI_Message.h"

namespace
{
	std::unique_ptr<std::thread> TextureThread;
	std::unique_ptr<std::thread> ModelThread;
	std::unique_ptr<std::thread> SpriteThread;
}

SceneLoad::SceneLoad()
{
	m_LoadProcess = 0;
	m_LoadProgress = 0;
	m_CurrentCount = 0;
	m_ThreadEnable = false;
}

void SceneLoad::Init()
{

	m_LoadProcess = 3;	//スレッドの総数 

	//-- ロード画面セット --
	{
		TextureManager::Load(TextureID::BG01, "data/texture/bg.png");
		auto obj = Object::Create("LoadCover");
		auto poly = obj->AddComponent<CPolygon>();
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

	//単騎ロード(バグ回避)
	LoadEffect();

	TextureThread = std::make_unique<std::thread>([this]() {this->LoadTexture(); });	//テクスチャ読み込み開始
	ModelThread = std::make_unique<std::thread>([this]() {this->LoadModel(); });	//モデル読み込み開始
	SpriteThread = std::make_unique<std::thread>([this]() {this->LoadSprite(); });	//スプライトデータ読み込み開始
}

void SceneLoad::Update()
{
	if (m_CurrentCount++ > MinCount && m_LoadProcess <= m_LoadProgress.load())
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

	SceneBase::Uninit();
}

void SceneLoad::LoadTexture()
{
	IG::MessageManager::DrawSystemLog("TextureLoadStart");

	TextureManager::Load(TextureID::NullTex, "data/texture/NullTex.jpg");
	TextureManager::Load(TextureID::Field01, "data/texture/Field01.jpg");
	TextureManager::Load(TextureID::VillageItem, "data/texture/VillageItem.png");
	TextureManager::Load(TextureID::RoadGrass, "data/texture/Road_grass.png");
	TextureManager::Load(TextureID::TitleCursor, "data/texture/Title/Cursor.png");
	TextureManager::Load(TextureID::Start, "data/texture/Title/strHajimeru.png");
	TextureManager::Load(TextureID::End, "data/texture/Title/strOwaru.png");
	TextureManager::Load(TextureID::TitleLogo, "data/texture/Title/TitleLogo2.png");
	TextureManager::Load(TextureID::TargetCursor, "data/texture/Target/TargetCursor.png");
	TextureManager::Load(TextureID::TargetPoint, "data/texture/Target/TargetPoint.png");
	TextureManager::Load(TextureID::UI_Ball, "data/texture/gameUI/ui_Ball.png");
	TextureManager::Load(TextureID::UI_TankBlue, "data/texture/gameUI/icon_Blue.png");
	TextureManager::Load(TextureID::UI_TankRed, "data/texture/gameUI/icon_Red.png");
	TextureManager::Load(TextureID::FieldTex, "data/texture/Field/WhiteTiles07_1K_AO.png");
	TextureManager::Load(TextureID::game_Start, "data/texture/gameUI/txtStart.png");
	TextureManager::Load(TextureID::game_Game, "data/texture/gameUI/txtGame.png");
	TextureManager::Load(TextureID::game_Clear, "data/texture/gameUI/txtClear.png");
	TextureManager::Load(TextureID::game_Failed, "data/texture/gameUI/txtFailed.png");
	TextureManager::Load(TextureID::minimap_Block, "data/texture/minimap/minimap_Block.png");
	TextureManager::Load(TextureID::minimap_Niedle, "data/texture/minimap/minimap_Niedle.png");
	TextureManager::Load(TextureID::minimap_Player, "data/texture/minimap/minimap_Player.png");
	TextureManager::Load(TextureID::minimap_Enemy, "data/texture/minimap/minimap_Enemy.png");
	TextureManager::Load(TextureID::Title_Circle, "data/texture/Title/Circle.png");
	TextureManager::Load(TextureID::Title_Wood, "data/texture/Title/Wood.png");
	TextureManager::Load(TextureID::Title_BG, "data/texture/Title/titlebg.png");
	TextureManager::Load(TextureID::Title_tank01Right, "data/texture/Title/title_tank01Right.png");
	TextureManager::Load(TextureID::Title_tank01Left, "data/texture/Title/title_tank01Left.png");
	TextureManager::Load(TextureID::Title_tank02Right, "data/texture/Title/title_tank02Right.png");
	TextureManager::Load(TextureID::Title_tank02Left, "data/texture/Title/title_tank02Left.png");
	TextureManager::Load(TextureID::Title_PressSpace, "data/texture/Title/pressspace.png");
	
	m_LoadProgress++;
	IG::MessageManager::DrawSystemLog("TextureLoadOK");
}

void SceneLoad::LoadModel()
{
	IG::MessageManager::DrawSystemLog("ModelLoadStart");

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
	ModelManager::Load(ModelID::Player_Head, "data/model/Tank/player/PlayerHead.fbx");
	ModelManager::Load(ModelID::Player_Body, "data/model/Tank/player/PlayerBody.fbx");
	ModelManager::Load(ModelID::Enemy01_Head, "data/model/Tank/enemy01/Enemy01Head.fbx");
	ModelManager::Load(ModelID::Enemy01_Body, "data/model/Tank/enemy01/Enemy01Body.fbx");
	ModelManager::Load(ModelID::Enemy02_Head, "data/model/Tank/enemy02/Enemy02Head.fbx");
	ModelManager::Load(ModelID::Enemy02_Body, "data/model/Tank/enemy02/Enemy02Body.fbx");
	ModelManager::Load(ModelID::Enemy03_Head, "data/model/Tank/enemy03/Enemy03Head.fbx");
	ModelManager::Load(ModelID::Enemy03_Body, "data/model/Tank/enemy03/Enemy03Body.fbx");
	ModelManager::Load(ModelID::Enemy04_Head, "data/model/Tank/enemy04/Enemy04Head.fbx");
	ModelManager::Load(ModelID::Enemy04_Body, "data/model/Tank/enemy04/Enemy04Body.fbx");
	ModelManager::Load(ModelID::WoodBlock, "data/model/WoodBlock/woodBlock.fbx");
	ModelManager::Load(ModelID::Niedle, "data/model/Niedle/SilverNiedle.fbx");
	
	IG::MessageManager::DrawSystemLog("ModelLoadOK");
	m_LoadProgress++;
}

void SceneLoad::LoadSprite()
{
	IG::MessageManager::DrawSystemLog("SpriteLoadStart");

	SpriteManager::Load(SpriteType::VillageItem, "data/SpriteData/VillageItem.csv", TextureID::VillageItem, 512, 512);
	m_LoadProgress++;
	IG::MessageManager::DrawSystemLog("SpriteLoadOK");
}

void SceneLoad::LoadEffect()
{
	IG::MessageManager::DrawSystemLog("EffectLoadStart");

	EffectManager::Load(EffectID::Test01, u"data/Effect/oura.efk");
	EffectManager::Load(EffectID::Hit01, u"data/Effect/Hit/Hit.efk");
	EffectManager::Load(EffectID::Explosion, u"data/Effect/ExpEffect.efk");
	EffectManager::Load(EffectID::HitEffect, u"data/Effect/HitEffect.efk");
	EffectManager::Load(EffectID::MuzzleFlash, u"data/Effect/MuzzleFlash.efk");
	
	IG::MessageManager::DrawSystemLog("EffectLoadOK");
	m_LoadProgress++;
}

