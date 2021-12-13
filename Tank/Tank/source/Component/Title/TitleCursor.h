//-------------------------
//	タイトルカーソル
//-- author --
// HatoriMasashi
//-------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Polygon\Polygon.h"

//-- クラス定義 --
class TitleCursor : public Component
{
public:
	TitleCursor();
	virtual ~TitleCursor() = default;

public:
	void Init() override;
	void Update() override;

	void SetPolygon(CPolygon* poly) { m_Poly = poly; }

private:
	CPolygon* m_Poly;
	int m_Current;
};
 
