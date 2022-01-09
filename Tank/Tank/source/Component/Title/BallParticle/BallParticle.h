//----------------------
//	ボールパーティクル
//-- author --
//	HatoriMasashi
//----------------------
#pragma once

//-- include --
#include "Component\Component.h"

//-- クラス定義 --
class CPolygon;
class BallParticle : public Component
{
public:
	BallParticle();
	virtual ~BallParticle() = default;

public:
	void Update() override;
	void SetPos(DirectX::XMFLOAT2 pos) { m_Pos = pos; }
	void SetRadius(float rad) { m_Radius = rad; }
	void SetSpeed(float spd) { m_Speed = spd; }
	void SetDir(DirectX::XMFLOAT2 dir) { m_Dir = dir; }
	void SetColor(DirectX::XMFLOAT3 color) { m_Color = color; }
	void SetAlpha(float alpha) { m_Alpha = alpha; }
	void SetUp(int time);	//現在情報を適応したポリゴン生成

private:
	DirectX::XMFLOAT2 m_Pos;
	float m_Radius;
	DirectX::XMFLOAT2 m_Dir;
	float m_Speed;
	DirectX::XMFLOAT3 m_Color;
	float m_Alpha;
	int m_FadeTime;
	int m_CurrentTime;
	CPolygon* m_Polygon;

	//-- static --
public:
	static void CreateParticle(SceneBase* scene,
		DirectX::XMFLOAT2 pos,DirectX::XMFLOAT3 color);
};
