//------------------------
//	エディットタイプ選択
//-- author --
//	HatoriMasashi
//------------------------

//-- include --
#include "EditType.h"
#include "Scene\SceneManager.h"
#include "Scene\ConstantScene.h"
#include "Component\Mouse\MouseInfo.h"
#include "MySystem\KeyInput\KeyInput.h"

//-- 静的メンバ --
PanelType EditType::CurrentSelect = PanelType::None;

/*
	コンストラクタ
*/
EditType::EditType()
{
	m_StringPolygon.clear();
	SetStringActive(false);
	m_ScreenPos = { 0.0f,0.0f };
	m_Size = { 10.0f,10.0f };
	m_Type = PanelType::None;
}

/*
	更新処理
*/
void EditType::Update()
{
	//-- 当たり判定 --
	auto mouse = ConstantScene::GetInstance().manager->FindObjectWithName("MouseManager")->GetComponent<MouseInfo>();
	auto mousePos = mouse->GetMousePos();	//マウス座標

	DirectX::XMFLOAT2 windowSize = { static_cast<float>(WindowInfo::m_ScreenWidth),static_cast<float>(WindowInfo::m_ScreenHeight) };
	DirectX::XMFLOAT2 screen = { m_ScreenPos.x + windowSize.x / 2,m_ScreenPos.y * -1 + windowSize.y / 2 };
	if (KeyInput::GetKeyPush(VK_LBUTTON))
	{
		if (mousePos.x > screen.x - m_Size.x / 2 && mousePos.x < screen.x + m_Size.x / 2 &&
			mousePos.y > screen.y - m_Size.y / 2 && mousePos.y < screen.y + m_Size.y / 2)
		{
			CurrentSelect = m_Type;
			SetStringActive(true);

			//-- 自身以外のタイプをfalseに --
			auto list = SceneManager::GetInstance().GetCurrentScene()->manager->FindObjectsWithName("guide");
			for (auto& obj : list)
			{
				auto et = obj->GetComponent<EditType>();
				if (et == nullptr || this == et) continue;
				et->SetStringActive(false);
			}
		}
	}
	else return;
}

/*
	文字列色変更
*/
void EditType::SetStringActive(bool state)
{
	static const DirectX::XMFLOAT3 active = { 1.0f,0.1f,0.1f };
	static const DirectX::XMFLOAT3 inactive = {1.0f,1.0f,1.0f};
	for (auto& obj : m_StringPolygon)
	{
		obj->SetColor(state ? active : inactive);
	}
}