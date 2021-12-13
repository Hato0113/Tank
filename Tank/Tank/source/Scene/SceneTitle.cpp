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

SceneTitle::SceneTitle()
{
	//none
}

void SceneTitle::Init()
{
	SceneBase::Init();

	{	
		auto obj = Object::Create("test");
		manager->Add(obj);
		obj = Object::Create("test");
		manager->Add(obj);
	}
	{	//背景
		auto obj = Object::Create("BG");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ static_cast<float>(WindowInfo::m_ScreenWidth),static_cast<float>(WindowInfo::m_ScreenHeight) });
		poly->SetColor({ 0.0f,0.0f,0.0f });
		manager->Add(obj);
	}
	{	//ロゴ
		auto obj = Object::Create("Logo");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ 1024.0f,512.0f });
		poly->SetTex(TextureManager::Get(TextureID::TitleLogo));
		poly->SetPos({ 0.0f,200.0f });
		manager->Add(obj);
	}
	{
		auto obj = Object::Create("start");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ 1024.0f / 2,512.0f / 2 });
		poly->SetTex(TextureManager::Get(TextureID::Start));
		poly->SetPos({ 0.0f,-50.0f });
		manager->Add(obj);
	}
	{
		auto obj = Object::Create("end");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ 1024.0f / 2,512.0f / 2 });
		poly->SetTex(TextureManager::Get(TextureID::End));
		poly->SetPos({ 0.0f,-200.0f });
		manager->Add(obj);
	}
	{	//カーソル
		auto obj = Object::Create("cursor");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ 256.0f / 2,256.0f / 2 });
		poly->SetTex(TextureManager::Get(TextureID::TitleCursor));
		auto cursor = obj->AddComponent<TitleCursor>();
		cursor->SetPolygon(poly);
		manager->Add(obj);
	}
	{
		auto obj = Object::Create("fonttest");
		FontManager::CreateString(obj, "TestFont", { 0.0f, 0.0f });
		//auto poly = obj->AddComponent<CPolygon>();
		//poly->SetSize({ 32.0f,64.0f });
		//poly->SetTex(FontManager::GetFont('*'));
		manager->Add(obj);
	}
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
}

