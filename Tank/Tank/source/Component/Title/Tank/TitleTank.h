//--------------------
//	タイトルタンク
//-- author --
//	HatoriMasashi
//--------------------
#pragma once

//-- include --
#include "Component\Component.h"

//-- クラス定義 --
class CPolygon;
class TitleTank : public Component
{
public:
	TitleTank();
	virtual ~TitleTank() = default;

	void Update();
	void SetUp(DirectX::XMFLOAT2 defPos, int dir,int type);
private:
	void ShotBullet();

private:
	int m_Dir;
	int m_Type;
	DirectX::XMFLOAT2 m_DefPos;
	DirectX::XMFLOAT2 m_Pos;
	CPolygon* m_Polygon;

	int m_Form;
	int m_FormCount;

	int m_ShotDiray;
};