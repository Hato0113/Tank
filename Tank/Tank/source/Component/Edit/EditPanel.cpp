//------------------------------
//	エディットで使用するパネル
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "EditPanel.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "Component\Mouse\MouseInfo.h"
#include "Scene\ConstantScene.h"
#include "EditType.h"

namespace
{
	const DirectX::XMFLOAT2 PanelSize = { 60.0f,60.0f };
	struct ChangePanelInfo
	{
		PanelType type;
		DirectX::XMFLOAT3 color;
	};
	const std::vector<ChangePanelInfo> panelInfo
	{
		{PanelType::None,{1.0f,1.0f,1.0f}},				//白
		{PanelType::Player,{0.0f,0.0f,1.0f}},			//青
		{PanelType::EnemyNormal,{1.0f,0.0f,1.0f}},		//マゼンタ
		{PanelType::EnemyQuickly,{0.9f,0.75f,0.5f}},	//茶色
		{PanelType::EnemyRapidFire,{1.0f,1.0f,0.0f}},	//黄色
		{PanelType::EnemyStrong,{0.0f,1.0f,0.0f}},		//緑
		{PanelType::Wall,{0.45f,0.37f,0.25f}},			//濃い茶色
		{PanelType::Niedle,{0.7f,0.7f,0.7f}},			//灰色
	};
}

/*
	コンストラクタ
*/
EditPanel::EditPanel()
{
	m_PanelType = PanelType::None;
	m_Coord = { 0,0 };
	m_ScreenPos = { 0.0f,0.0f };
	m_CurrentType = 0;
}

/*
	更新
	パラメータ無し
*/
void EditPanel::Update()
{
	auto mouse = ConstantScene::GetInstance().manager->FindObjectWithName("MouseManager")->GetComponent<MouseInfo>();
	auto mousePos = mouse->GetMousePos();	//マウス座標

	DirectX::XMFLOAT2 windowSize = { static_cast<float>(WindowInfo::m_ScreenWidth),static_cast<float>(WindowInfo::m_ScreenHeight) };
	DirectX::XMFLOAT2 screen = { m_ScreenPos.x + windowSize.x / 2,m_ScreenPos.y * -1 + windowSize.y / 2 };
	if (KeyInput::GetKeyPush(VK_LBUTTON))
	{
		if (mousePos.x > screen.x - PanelSize.x / 2 && mousePos.x < screen.x + PanelSize.x / 2 &&
			mousePos.y > screen.y - PanelSize.y / 2 && mousePos.y < screen.y + PanelSize.y / 2)
		{
			//m_CurrentType += 1;
			//if (panelInfo.size() <= m_CurrentType)
			//	m_CurrentType = 0;

			//-- パネル情報変化 --
			//m_PanelType = panelInfo[m_CurrentType].type;
			m_PanelType = EditType::GetCurrentSelect();
			for (auto info : panelInfo)
			{
				if (info.type == m_PanelType)
					m_Polygon->SetColor(info.color);
			}
			//m_Polygon->SetColor(panelInfo[m_CurrentType].color);
		}
	}
	else return;
}

/*
	パネル情報セット
	引数 : パネルタイプ
*/
void EditPanel::SetType(PanelType type)
{
	for (int i = 0; i < panelInfo.size(); i++)
	{
		if (panelInfo[i].type == type)
		{
			m_PanelType = type;
			m_Polygon->SetColor(panelInfo[i].color);
			m_CurrentType = i;
			break;
		}
	}
}

