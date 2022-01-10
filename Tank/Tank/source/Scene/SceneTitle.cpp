//------------------------------
//	タイトルシーンクラス
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "SceneTitle.h"
#include "Objects\Object.h"
#include "Component\Polygon\Polygon.h"
#include "MySystem\Texture\TextureManager.h"
#include "Component\Title\TitleCursor.h"
#include "MySystem\FontManager\FontManager.h"
#include "Component\Title\Ball\TitleBall.h"
#include "Component\Title\Tank\TitleTank.h"
#include "System\Sound.h"

SceneTitle::SceneTitle()
{
	//none
}

void SceneTitle::Init()
{
	SceneBase::Init();

	{	//背景
		const DirectX::XMFLOAT2 pos[2] = {
			{512.0f,0.0f},
			{-512.0f,0.0f},
		};
		for (int i = 0; i < 2; i++)
		{
			auto obj = Object::Create("bgWood");
			auto poly = obj->AddComponent<CPolygon>();
			poly->SetPos(pos[i]);
			poly->SetSize({ 1024.0f,1024.0f });
			poly->SetTex(TextureManager::Get(TextureID::Title_Wood));
			manager->Add(obj);
		}
	}
	if(true){	//背景
		auto obj = Object::Create("BG");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ static_cast<float>(WindowInfo::m_ScreenWidth) - 100.0f,static_cast<float>(WindowInfo::m_ScreenHeight) - 100.0f });
		poly->SetColor({ 1.0f,1.0f,1.0f });
		poly->SetTex(TextureManager::Get(TextureID::Title_BG));
		manager->Add(obj);
	}
	if (true) {	//ロゴ
		auto obj = Object::Create("Logo");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ 896.0f,504.0f });
		poly->SetTex(TextureManager::Get(TextureID::TitleLogo));
		poly->SetPos({ 0.0f,0.0f });
		manager->Add(obj);
	}
	if (true) {	//カーソル
		auto obj = Object::Create("cursor");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetPos({ 70.0f,-260.0f });
		poly->SetSize({ 1610.0f / 2,316.0f / 2 });
		poly->SetTex(TextureManager::Get(TextureID::Title_PressSpace));
		auto cursor = obj->AddComponent<TitleCursor>();
		cursor->SetPolygon(poly);
		manager->Add(obj);
	}
	if(false){	
		auto obj = Object::Create("ball");
		auto ball = obj->AddComponent<TitleBall>();
		ball->SetPos({ 0.0f,0.0f });
		ball->SetColor({ 0.0f,0.0f,1.0f });
		ball->SetAlpha(0.6f);
		ball->SetRadius(70.0f);
		ball->SetDir({ 0.5f,1.0f });
		ball->SetSpeed(5.0f);
		ball->SetUp();
		manager->Add(obj);
	}
	if(false){
		auto obj = Object::Create("ball");
		auto ball = obj->AddComponent<TitleBall>();
		ball->SetPos({ 200.0f,0.0f });
		ball->SetColor({ 0.0f,1.0f,0.0f });
		ball->SetAlpha(0.6f);
		ball->SetRadius(70.0f);
		ball->SetDir({ 1.0f,0.5f });
		ball->SetSpeed(5.0f);
		ball->SetUp();
		manager->Add(obj);
	}
	{
		auto obj = Object::Create("tank");
		auto tank = obj->AddComponent<TitleTank>();
		tank->SetUp({ 0.0f,-270.0f }, 1, 0);
		manager->Add(obj);
	}
	{
		auto obj = Object::Create("tank");
		auto tank = obj->AddComponent<TitleTank>();
		tank->SetUp({ 0.0f,270.0f }, -1, 1);
		manager->Add(obj);
	}

	//BGM
	CSound::Play(BGM_TITLE);
	CSound::SetVolume(0.2f);
}

void SceneTitle::Update()
{
	SceneBase::Update();
}

void SceneTitle::Draw()
{
	SceneBase::Draw();
}

void SceneTitle::Uninit()
{
	SceneBase::Uninit();
	CSound::Stop(BGM_TITLE);
}

