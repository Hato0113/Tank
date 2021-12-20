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
		auto make = [&mana](DirectX::XMFLOAT2 pos, DirectX::XMFLOAT3 color, std::string str)
		{
			auto offset = pos;
			auto obj = Object::Create("guide");
			auto poly = obj->AddComponent<CPolygon>();
			poly->SetPos(offset);
			poly->SetSize({ 30.0f, 30.0f });
			poly->SetColor(color);
			FontManager::CreateString(obj, str, { offset.x + 30.0f,offset.y }, 0.5f);
			mana->Add(obj);
		};

		make({ 420.0f,300.0f }, { 1.0f,1.0f,1.0f }, " : None");
		make({ 420.0f,260.0f }, { 1.0f,0.0f,0.0f }, " : Player");
		make({ 420.0f,220.0f }, { 1.0f,1.0f,0.0f }, " : Enemy");
		make({ 420.0f,180.0f }, { 1.0f,0.0f,1.0f }, " : Wall");
		make({ 420.0f,140.0f }, { 0.0f,1.0f,0.0f }, " : Hole");
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
