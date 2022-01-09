//----------------------
//	ボールパーティクル
//-- author --
//	HatoriMasashi
//----------------------

//-- include --
#include "BallParticle.h"
#include "Component\Polygon\Polygon.h"
#include "MySystem\Texture\TextureManager.h"

namespace
{
	const float startAlpha = 0.8f;

	const int createNum = 20;
}

/*
	コンストラクタ
*/
BallParticle::BallParticle()
{
	m_Pos = { 0.0f,0.0f };
	m_Radius = 0.0f;
	m_Dir = { 0.0f,0.0f };
	m_Speed = 0.0f;
	m_Color = { 1.0f,1.0f,1.0f };
	m_Alpha = 1.0f;
	m_FadeTime = 0;
	m_CurrentTime = 0;
	m_Polygon = nullptr;
}

/*
	セットアップ関数
	param : 継続時間
*/
void BallParticle::SetUp(int time)
{
	auto poly = parent->AddComponent<CPolygon>();
	poly->SetPos(m_Pos);
	poly->SetSize({ m_Radius * 2,m_Radius * 2 });
	poly->SetColor(m_Color);
	poly->SetAlpha(startAlpha);
	poly->SetTex(TextureManager::Get(TextureID::Title_Circle));
	m_Polygon = poly;

	m_FadeTime = time;
}

/*
	更新処理
	パラメータ無し
*/
void BallParticle::Update()
{
	//α値変更
	float alpha = 1.0f - (m_CurrentTime / static_cast<float>(m_FadeTime));
	alpha *= startAlpha;
	m_Polygon->SetAlpha(alpha);

	//移動
	m_Pos.x += m_Dir.x * m_Speed;
	m_Pos.y += m_Dir.y * m_Speed;
	m_Polygon->SetPos(m_Pos);

	//速度変化
	m_Speed *= 0.99f;

	//時間加算
	m_CurrentTime++;
}

/*
	パーティクル生成
	param : 親オブジェクトポインタ
*/
void BallParticle::CreateParticle(SceneBase* scene,DirectX::XMFLOAT2 pos,DirectX::XMFLOAT3 color)
{
	auto obj = Object::Create("particle");
	obj->SetLifeTime(180);
	float angle = 0.0f;
	for (int i = 0; i < createNum; i++)
	{
		auto par = obj->AddComponent<BallParticle>();
		par->SetPos(pos);
		par->SetAlpha(startAlpha);
		par->SetColor(color);
		par->SetRadius(20.0f);
		par->SetDir({ cosf(angle),sinf(angle) });
		par->SetSpeed(2.0f);
		par->SetUp(180);

		angle += DirectX::XM_PI / 180.0f * 360.0f / createNum;
	}

	scene->manager->Add(obj);
}


