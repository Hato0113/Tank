//----------------------
//	ポリゴンフェード
//-- author --
//	HatoriMasashi
//----------------------

//-- include --
#include "PolygonFade.h"

/*
	コンストラクタ
*/
PolygonFade::PolygonFade()
{
	m_FadeState = false;
	m_Current = -1;
	m_Max = -1;
}

/*
	更新
	パラメータ無し
*/
void PolygonFade::Update()
{
	if (!m_FadeState)
	{
		m_Current--;
		if (m_Current == 0)
		{
			m_FadeState = true;
		}
	}
	else
	{
		float alpha = (m_Max - m_Current) / static_cast<float>(m_Max);
		for (auto poly : m_Polygons)
		{
			poly->SetAlpha(alpha);
		}
		m_Current++;
	}

}

/*
	フェードセッティング
	param : Start 開始フレーム
	param : Count 継続フレーム
*/
void PolygonFade::SetFade(int Start, int Count)
{
	m_FadeState = false;
	m_Current = Start;
	m_Max = Count;

	//-- ポリゴン探索 --
	m_Polygons = parent->GetComponents<CPolygon>();
}
