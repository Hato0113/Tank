//------------------------------
//	敵生存数UI
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "EnemyTankUI.h"
#include "MySystem\FontManager\FontManager.h"
#include "MySystem\Texture\TextureManager.h"

namespace
{
	const DirectX::XMFLOAT2 polygonDefPos = { -200.0f,-320.0f };
}

/*
	コンストラクタ
*/
EnemyTankUI::EnemyTankUI()
{
	m_EnemyNum = 0;
	m_PrevNum = 0;
	m_StrObj = nullptr;
}

/*
	初期化
	パラメータ無し
*/
void EnemyTankUI::Init()
{
	//-- ポリゴン生成 --
	auto poly = parent->AddComponent<CPolygon>();
	poly->SetTex(TextureManager::Get(TextureID::UI_TankRed));
	poly->SetSize({ 80.0f,80.0f });
	poly->SetPos({ polygonDefPos.x - 100.0f,polygonDefPos.y });
	poly->SetLayer(Layer::Front2D);
}

/*
	更新
	パラメータ無し
*/
void EnemyTankUI::Update()
{
	//-- 敵数検知 --
	auto EnemyList = parent->GetScene()->manager->FindObjectsWithTag("Enemy");
	m_EnemyNum = static_cast<int>(EnemyList.size());
	if (m_EnemyNum == m_PrevNum) return;

	//-- 文字列更新 --
	{
		if (m_StrObj) m_StrObj->SetEnable(false);
		std::string str = "x ";
		str += std::to_string(m_EnemyNum);
		auto obj = Object::Create("enemyNumStr");
		FontManager::CreateString(obj, str, polygonDefPos, 2.0f, { 1.0f,0.9f,0.8f,1.0f });
		parent->GetScene()->manager->Add(obj);
		m_StrObj = obj;
		m_PrevNum = m_EnemyNum;
	}

}

