//-----------------------------
//	ポリゴンコンポーネント
//-- author --
//	HatoriMasashi
//-----------------------------

//-- include --
#include "Polygon.h"

CPolygon::CPolygon()
{
	m_Pos = { 0.0f,0.0f };
	m_Size = { 10.0f,10.0f };
	m_UV = { 0.0f,0.0f };
	m_FrameSize = { 1.0f,1.0f };
	m_Color = { 1.0f,1.0f,1.0f };
	m_AngleZ = 0.0f;
	m_Alpha = 1.0f;
	m_pTex = nullptr;
	m_useShader = "PS_Polygon";

	//-- レイヤー変更 --
	Component::SetLayer(Layer::Back2D);
}
CPolygon::~CPolygon()
{
	//none
}

void CPolygon::Draw()
{
	PolygonManager::Draw(this);
}

