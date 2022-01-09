//------------------------------
//	レベルエディターシーンクラス
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "SceneLevelEditor.h"
#include "Component\Edit\EditPanel.h"
#include "MySystem\LevelManager\MapManager.h"
#include "MySystem\FontManager\FontManager.h"
#include "Component\Edit\EditType.h"
#include "MySystem\Texture\TextureManager.h"
/*
	コンストラクタ
*/
SceneLevelEditor::SceneLevelEditor()
{
	//none
}

/*
	初期化
	パラメータ無し
*/
void SceneLevelEditor::Init()
{
	SceneBase::Init();

	{	//背景
		auto obj = Object::Create("bg");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetTex(TextureManager::Get(TextureID::Title_BG));
		poly->SetColor({ 0.4f,0.4f,0.4f });
		poly->SetSize({ static_cast<float>(WindowInfo::m_ScreenWidth),static_cast<float>(WindowInfo::m_ScreenHeight) });
		manager->Add(obj);
	}

	{	//パネル生成
		auto mana = SceneBase::manager;
		const DirectX::XMFLOAT2 Size = { 60.0f,60.0f };
		auto make = [&mana,&Size](DirectX::XMFLOAT2 pos, DirectX::XMINT2 coord)
		{
			auto obj = Object::Create("MapPanel");
			obj->transform->SetTag("Panel");
			auto panel = obj->AddComponent<EditPanel>();
			auto poly = obj->AddComponent<CPolygon>();
			poly->SetPos(pos);
			poly->SetSize({Size.x, Size.y});
			panel->SetCoord(coord);
			panel->SetPolygon(poly);
			panel->SetScreenPos(pos);
			mana->Add(obj);
		};

		const DirectX::XMFLOAT2 defPos = { static_cast<float>(-WindowInfo::m_ScreenWidth) / 2 + Size.x,
										   static_cast<float>(WindowInfo::m_ScreenHeight) / 2 - Size.y};
		const float margin = 5.0f;
		for (int y = 0; y < MapManager::MapSize.y; y++)
		{
			for (int x = 0; x < MapManager::MapSize.x; x++)
			{
				make({ defPos.x + x * (Size.x + margin),defPos.y - y * (Size.y + margin) }, { x,y });
			}
		}
	}

	{	//エディターガイド
		auto mana = SceneBase::manager;
		auto make = [&mana](DirectX::XMFLOAT2 pos, DirectX::XMFLOAT3 color, std::string str,PanelType type)
		{
			auto offset = pos;
			auto obj = Object::Create("guide");
			auto poly = obj->AddComponent<CPolygon>();
			poly->SetPos(offset);
			poly->SetSize({ 30.0f, 30.0f });
			poly->SetColor(color);
			auto polygonList = FontManager::CreateString(obj, str, { offset.x + 30.0f,offset.y }, 0.5f);
			auto et = obj->AddComponent<EditType>();
			et->SetStringPolygon(polygonList);
			et->SetType(type);
			et->SetPos(offset);
			et->SetSize({ 30.0f,30.0f });
			mana->Add(obj);
		};

		make({ 420.0f,300.0f }, { 1.0f,1.0f,1.0f }, " : None",PanelType::None);
		make({ 420.0f,260.0f }, { 0.0f,0.0f,1.0f }, " : Player",PanelType::Player);
		make({ 420.0f,220.0f }, { 1.0f,0.0f,1.0f }, " : E_Normal",PanelType::EnemyNormal);
		make({ 420.0f,180.0f }, { 0.9f,0.75f,0.5f }, " : E_Quickly",PanelType::EnemyQuickly);
		make({ 420.0f,140.0f }, { 1.0f,1.0f,0.0f }, " : E_Rapid",PanelType::EnemyRapidFire);
		make({ 420.0f,100.0f }, { 0.0f,1.0f,0.0f }, " : E_Strong",PanelType::EnemyStrong);
		make({ 420.0f, 60.0f }, { 0.45f,0.37f,0.25f }, " : Wall",PanelType::Wall);
		make({ 420.0f, 20.0f }, { 0.7f,0.7f,0.7f }, " : Hole",PanelType::Niedle);
	}
}

/*
	更新
	パラメータ無し
*/
void SceneLevelEditor::Update()
{
	SceneBase::Update();
}

/*
	描画
	パラメータ無し
*/
void SceneLevelEditor::Draw()
{
	SceneBase::Draw();
}

/*
	終了処理
	パラメータ無し
*/
void SceneLevelEditor::Uninit()
{
	SceneBase::Uninit();
}
