//--------------------------
//	フェード管理
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Fade.h"

Fade::Fade()
{
	m_Polygon = nullptr;
	m_Event = FadeEvent::EventNone;
	m_Mode = FadeMode::None;	//停止
	m_CurrentTime = 0;
}

void Fade::Init()
{
	//-- フェード用ポリゴン生成 -
	m_Polygon = parent->AddComponent<CPolygon>();
	m_Polygon->SetSize({ static_cast<float>(WindowInfo::m_ScreenWidth),static_cast<float>(WindowInfo::m_ScreenHeight) });
	m_Polygon->SetAlpha(0.0f);
	m_Polygon->SetColor({ 0.0f,0.0f,0.0f });
}

void Fade::Update()
{
	if (m_Mode == FadeMode::None)
	{
		if (m_Event == FadeEvent::Middle)
		{
			m_CurrentTime = 0;	//フェードされていない場合はリセット
			m_Polygon->SetAlpha(static_cast<float>(m_CurrentTime) / MaxTime);
		}
		m_Event = FadeEvent::EventNone;
		
		return;
	}

	switch (m_Mode)
	{
	case FadeMode::FadeIn:
		m_CurrentTime++;
		if (m_CurrentTime == MaxTime)
		{
			m_Mode = FadeMode::None;
			m_Event = FadeEvent::Middle;
		}
		break;
	case FadeMode::FadeOut:
		m_CurrentTime--;
		if (m_CurrentTime == 0)
		{
			m_Mode = FadeMode::None;
			m_Event = FadeEvent::End;
		}

	default:
		break;
	}

	//-- アルファ値調整 --
	m_Polygon->SetAlpha(static_cast<float>(m_CurrentTime) / MaxTime);
}

void Fade::SetUp(FadeMode mode)
{
	if (m_Mode != FadeMode::None) return;

	//-- セットアップ --
	m_Mode = mode;
	m_Event = FadeEvent::EventNone;

	switch (mode)
	{
	case FadeMode::FadeIn:
		m_CurrentTime = 0;	
		break;
	case FadeMode::FadeOut:
		m_CurrentTime = MaxTime;
		break;
	default:
		break;
	}
}