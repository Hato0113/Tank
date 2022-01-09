//--------------------
//	タイトルタンク
//-- author --
//	HatoriMasashi
//--------------------

//-- include --
#include "TitleTank.h"
#include "Component\Polygon\Polygon.h"
#include "MySystem\Texture\TextureManager.h"
#include "Component\Title\Ball\TitleBall.h"

namespace
{
	const float Xmax = 1280.0f / 2;
	const float Xmin = -1280.0f / 2;
	const float size = 96.0f;
	const float speed = 1.5f;
	const int formCount = 30;
	const float formDifPos = 10.0f;	//上下に動かす際の差分

	const int ShotMin = 60;
	const int ShotWidth = 120;

	const DirectX::XMFLOAT3 ballColors[] =
	{
		{1.0f,0.0f,0.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,1.0f,0.0f},
		{1.0f,1.0f,0.0f},
		{0.0f,1.0f,1.0f},
		{1.0f,0.0f,1.0f},
		{0.5f,1.0f,0.0f},
		{1.0f,0.25f,0.0f},
		{0.5f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
	};
}


/*
	コンストラクタ
*/
TitleTank::TitleTank()
{
	m_Dir = -1;
	m_Type = 0;
	m_Pos = m_DefPos = { 0.0f,0.0f };
	m_Polygon = nullptr;

	m_Form = 0;
	m_FormCount = formCount;

	m_ShotDiray = ShotMin + rand() % ShotWidth;
}

/*
	セットアップ
	param : デフォルト座標
	param : 方向 -1 左 : 1 右
	param : タイプ 0,1
*/
void TitleTank::SetUp(DirectX::XMFLOAT2 defpos, int dir,int type)
{
	m_Pos = m_DefPos = defpos;
	m_Dir = dir;
	m_Type = type;

	auto poly = parent->AddComponent<CPolygon>();
	poly->SetPos(m_DefPos);
	poly->SetSize({ size,size });
	if (type == 0)
		poly->SetTex(TextureManager::Get(TextureID::Title_tank01Right));
	else if (type == 1)
		poly->SetTex(TextureManager::Get(TextureID::Title_tank02Left));
	m_Polygon = poly;
}

/*
	更新
*/
void TitleTank::Update()
{
	//-- 移動 --
	int prevDir = m_Dir;
	m_Pos.x += speed * m_Dir;
	if (m_Pos.x > Xmax - size)
		m_Dir *= -1;
	else if (m_Pos.x < Xmin + size)
		m_Dir *= -1;

	//-- テクスチャ更新 --
	if (m_Dir != prevDir)
	{
		if (m_Type == 0)		//下
		{
			if (m_Dir == 1)	//右
			{
				m_Polygon->SetTex(TextureManager::Get(TextureID::Title_tank01Right));
			}
			else if (m_Dir == -1)
			{
				m_Polygon->SetTex(TextureManager::Get(TextureID::Title_tank01Left));
			}
		}
		else if (m_Type == 1)	//上
		{
			if (m_Dir == 1)	//右
			{
				m_Polygon->SetTex(TextureManager::Get(TextureID::Title_tank02Right));
			}
			else if (m_Dir == -1)
			{
				m_Polygon->SetTex(TextureManager::Get(TextureID::Title_tank02Left));
			}
		}
	}

	//-- フォーム更新 --
	if (m_FormCount) m_FormCount--;
	if (!m_FormCount)
	{
		m_FormCount = formCount;
		m_Form ^= 1;
		m_Pos.y = m_DefPos.y + m_Form * formDifPos * (m_Type ? -1 : 1);
	}

	m_Polygon->SetPos(m_Pos);

	//-- ショット --
	if (m_ShotDiray) m_ShotDiray--;
	if (!m_ShotDiray)
	{
		ShotBullet();

		m_ShotDiray = ShotMin + rand() % ShotWidth;
	}
}

/*
	弾発射
*/
void TitleTank::ShotBullet()
{
	auto obj = Object::Create("ball");
	auto ball = obj->AddComponent<TitleBall>();

	auto pos = m_Pos;
	pos.x += m_Dir * 40.0f;
	pos.y += (m_Type ? -1 : 1) * 40.0f;

	ball->SetPos(pos);
	int c = rand() % _countof(ballColors);
	ball->SetColor(ballColors[c]);
	ball->SetAlpha(0.6f);
	ball->SetRadius(40.0f);
	DirectX::XMFLOAT2 dir;
	dir.x = m_Dir * 2.0f;
	dir.y = (m_Type ? -1.0f : 1.0f);

	ball->SetDir(dir);
	ball->SetSpeed(3.0f);
	ball->SetUp();
	parent->GetScene()->manager->Add(obj);
}