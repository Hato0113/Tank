//----------------------
//	ポリゴンフェード
//-- author --
//	HatoriMasashi
//----------------------
#pragma once

//-- include --
#include "Polygon.h"

//-- クラス定義 --
class PolygonFade : public Component
{
public:
	PolygonFade();
	virtual ~PolygonFade() = default;
public:
	void Update() override;

	void SetFade(int Start, int Count);	//フェードのセット
private:
	bool m_FadeState;
	int m_Current;
	int m_Max;
	std::vector<CPolygon*> m_Polygons;	//対象のポリゴン
};

