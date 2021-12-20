//------------------------------
//	エディットで使用するパネル
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "MySystem\LevelManager\LevelManager.h"
#include "Component\Polygon\Polygon.h"

//-- クラス定義 --
class EditPanel :public Component
{
public:
	EditPanel();
	virtual ~EditPanel() = default;

	void Update() override;
	void SetPolygon(CPolygon* poly) { m_Polygon = poly; }
	void SetCoord(DirectX::XMINT2 coord) { m_Coord = coord; }
	void SetScreenPos(DirectX::XMFLOAT2 pos) { m_ScreenPos = pos; }
	void SetType(PanelType type);

	DirectX::XMINT2 GetCoord() { return m_Coord; }
	PanelType GetType() { return m_PanelType; }

private:
	PanelType m_PanelType;		//パネルタイプ
	DirectX::XMINT2 m_Coord;	//パネル上の座標
	DirectX::XMFLOAT2 m_ScreenPos;
	CPolygon* m_Polygon;
	int m_CurrentType;
};

