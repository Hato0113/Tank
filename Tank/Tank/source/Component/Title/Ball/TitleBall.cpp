//--------------------------
//	タイトルで跳ねるボール
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "TitleBall.h"
#include "Component\Polygon\Polygon.h"
#include "MySystem\Texture\TextureManager.h"
#include "Component\Title\BallParticle\BallParticle.h"

namespace
{
	const DirectX::XMFLOAT2 minWall = {-640.0f + 50.0f, -360.0f + 50.0f};
	const DirectX::XMFLOAT2 maxWall = { 640.0f - 50.0f,360.0f - 50.0f };
	const int maxHit = 5;
	const float bounceCoeffictent = 0.9f;
}

/*
	コンストラクタ
*/
TitleBall::TitleBall()
{
	m_Pos = { 0.0f,0.0f };
	m_Radius = 5.0f;
	m_Dir = { 1.0f,0.0f };
	m_Speed = 1.0f;
	m_Color = { 1.0f,1.0f,1.0f };
	m_Alpha = 1.0f;
	m_HitCount = 0;
	m_Polygon = nullptr;
}

/*
	初期化
	パラメータ無し
*/
void TitleBall::Init()
{
	parent->transform->SetTag("Ball");	//タグ設定
}

/*
	セットアップ関数
	パラメータ無し
*/
void TitleBall::SetUp()
{
	auto poly = parent->AddComponent<CPolygon>();
	poly->SetPos(m_Pos);
	poly->SetTex(TextureManager::Get(TextureID::Title_Circle));
	poly->SetSize({ m_Radius * 2,m_Radius * 2 });
	poly->SetAlpha(m_Alpha);
	poly->SetColor(m_Color);
	m_Mass = m_Radius * 2;
	m_Polygon = poly;
}

/*
	更新処理
	パラメータ無し
*/
void TitleBall::Update()
{
	//-- 移動 --
	m_Pos.x += m_Dir.x * m_Speed;
	m_Pos.y += m_Dir.y * m_Speed;

	//-- 判定 --
	bool hit = false;
	{	//壁との判定
		if (m_Pos.x + m_Radius >= maxWall.x)	//右壁
		{
			m_Pos.x -= m_Dir.x * m_Speed;
			m_Dir.x *= -1;
			hit = true;
		}
		else if (m_Pos.x - m_Radius <= minWall.x)	//左壁
		{
			m_Pos.x -= m_Dir.x * m_Speed;
			m_Dir.x *= -1;
			hit = true;
		}
		if (m_Pos.y + m_Radius >= maxWall.y)	//上壁
		{
			m_Pos.y -= m_Dir.y * m_Speed;
			m_Dir.y *= -1;
			hit = true;
		}
		else if (m_Pos.y - m_Radius <= minWall.y)	//下壁
		{
			m_Pos.y -= m_Dir.y * m_Speed;
			m_Dir.y *= -1;
			hit = true;
		}
	}
	{	//他のボールとの判定
		//情報取得
		auto ballList = parent->GetScene()->manager->FindObjectsWithTag("Ball");
		for (auto obj : ballList)
		{
			if (obj == this->parent) continue;	//自分自身
			auto ball = obj->GetComponent<TitleBall>();
			//-- 当たったかな否か --
			if (CollisionCircle(this, ball))
			{
				//-- 跳ね返り判定 --
				BounceCircle(this, ball);
				hit = true;
			}
		}
	}

	//-- 当たった回数に寄っての判定 --
	if ((m_HitCount += (hit ? 1 : 0)) >= maxHit)
	{
		//オブジェクト削除
		parent->SetState(false);
		//パーティクル生成
		BallParticle::CreateParticle(parent->GetScene(), m_Pos, m_Color);
	}

	//-- ポリゴン情報更新 --
	m_Polygon->SetPos(m_Pos);
	m_Polygon->SetAlpha(m_Alpha);
	m_Polygon->SetColor(m_Color);
}


/*
	方向セット
	param : 方向
*/
void TitleBall::SetDir(DirectX::XMFLOAT2 dir)
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat2(&dir);
	vec = DirectX::XMVector2Normalize(vec);
	DirectX::XMStoreFloat2(&m_Dir, vec);
}

/*
	円同士の当たり判定
	param : 自身の情報
	param : 相手の情報
	return : あったか否か
*/
bool TitleBall::CollisionCircle(TitleBall* my, TitleBall* other)
{
	float distance = powf(my->m_Pos.x - other->m_Pos.x, 2) + powf(my->m_Pos.y - other->m_Pos.y, 2);
	float radius = my->m_Radius + other->m_Radius;
	if (powf(radius, 2) > distance)
		return true;
	else return false;
}

/*
	跳ね返り判定
	param : 自身の情報
	param : 相手の情報
*/
void TitleBall::BounceCircle(TitleBall* my, TitleBall* other)
{
	auto myDir = my->m_Dir;
	auto otherDir = other->m_Dir;

	my->m_Dir.x = ((my->m_Mass - bounceCoeffictent * other->m_Mass) * myDir.x + (1.0f + bounceCoeffictent) * other->m_Mass * otherDir.x) / (my->m_Mass + other->m_Mass);
	my->m_Dir.y = ((my->m_Mass - bounceCoeffictent * other->m_Mass) * myDir.y + (1.0f + bounceCoeffictent) * other->m_Mass * otherDir.y) / (my->m_Mass + other->m_Mass);
	other->m_Dir.x = ((other->m_Mass - bounceCoeffictent * my->m_Mass) * otherDir.x + (1.0f + bounceCoeffictent) * my->m_Mass * myDir.x) / (my->m_Mass + other->m_Mass);
	other->m_Dir.y = ((other->m_Mass - bounceCoeffictent * my->m_Mass) * otherDir.y + (1.0f + bounceCoeffictent) * my->m_Mass * myDir.y) / (my->m_Mass + other->m_Mass);
}
