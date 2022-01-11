//------------------------------
//	ボールUI
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "BallUI.h"
#include "MySystem\Resident\ResidentData.h"
#include "MySystem\Texture\TextureManager.h"

namespace
{
	const DirectX::XMFLOAT2 polygonDefPos = { 100.0f,-320.0f };
	const float space = 90.0f;
	const DirectX::XMFLOAT2 polygonSize = { 80.0f,80.0f };
}

/*
	コンストラクタ
*/
BallUI::BallUI()
{
	m_Balls.clear();
	m_PlayerWeapon = nullptr;
}

/*
	初期化
	パラメータ無し
*/
void BallUI::Init()
{
	for (int i = 0; i < ResidentDataManager::GetData().PlayerData.ShotMax; i++)
	{
		auto poly = parent->AddComponent<CPolygon>();
		poly->SetPos({ polygonDefPos.x + space * i,polygonDefPos.y });
		poly->SetTex(TextureManager::Get(TextureID::UI_Ball));
		poly->SetSize(polygonSize);
		poly->SetLayer(Layer::Front2D);
		m_Balls.push_back(poly);
	}
}

/*
	更新処理
	パラメータ無し
*/
void BallUI::Update()
{
	//-- プレイヤーの武器情報取得 --
	if (!m_PlayerWeapon)
	{
		auto player = parent->GetScene()->manager->FindObjectWithTag("Player");
		if (player)
			m_PlayerWeapon = player->GetComponent<NormalWeapon>();
		if (!m_PlayerWeapon) return;
	}

	//-- ポリゴンの更新 --
	{
		const float activeAlpha = 1.0f;
		const float inactiveAlpha = 0.3f;
		const int ActiveNum = m_PlayerWeapon->GetCurrentBulletNum();
		const int BallMax = static_cast<int>(m_Balls.size());
		for (int i = 0; i < m_Balls.size(); i++)
		{
			if (i < BallMax - ActiveNum)
				m_Balls[i]->SetAlpha(activeAlpha);
			else
				m_Balls[i]->SetAlpha(inactiveAlpha);
		}
	}
}

