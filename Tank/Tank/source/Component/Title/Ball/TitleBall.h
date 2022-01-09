//--------------------------
//	タイトルで跳ねるボール
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"

//-- クラス定義 --
class CPolygon;
class TitleBall : public Component
{
public:
	TitleBall();
	virtual ~TitleBall() = default;

public:
	void Init() override;
	void Update() override;

	void SetPos(DirectX::XMFLOAT2 pos) { m_Pos = pos; }
	void SetRadius(float rad) { m_Radius = rad; }
	void SetDir(DirectX::XMFLOAT2 dir);
	void SetSpeed(float spd) { m_Speed = spd; }
	void SetColor(DirectX::XMFLOAT3 color) { m_Color = color; }
	void SetAlpha(float alpha) { m_Alpha = alpha; }
	void SetUp();	//現在情報を適応したポリゴン生成

private:
	bool CollisionCircle(TitleBall*, TitleBall*);
	void BounceCircle(TitleBall*, TitleBall*);

private:
	DirectX::XMFLOAT2 m_Pos;
	float m_Radius;
	DirectX::XMFLOAT2 m_Dir;
	float m_Speed;
	DirectX::XMFLOAT3 m_Color;
	float m_Alpha;
	float m_Mass;
	int m_HitCount;		//当たった回数
	CPolygon* m_Polygon;

};


