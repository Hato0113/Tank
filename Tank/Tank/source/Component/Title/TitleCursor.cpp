//-------------------------
//	タイトルカーソル
//-- author --
// HatoriMasashi
//-------------------------

//-- include --
#include "TitleCursor.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "MySystem\Application.hpp"
#include "Scene\SceneManager.h"

namespace
{
	constexpr int Max = 2;
	constexpr DirectX::XMFLOAT2 Points[Max] = 
	{
		{-300.0f,-50.0f},
		{-300.0f,-200.0f},
	};
}

TitleCursor::TitleCursor()
{
	m_Current = 0;
	m_Poly = nullptr;
}

void TitleCursor::Init()
{

}

void TitleCursor::Update()
{
	//-- カーソル移動 --
	if (KeyInput::GetKeyPush('S'))
	{
		m_Current++;
		if (m_Current >= Max)
			m_Current = 0;
	}
	if (KeyInput::GetKeyPush('W'))
	{
		m_Current--;
		if (m_Current < 0)
			m_Current += Max;
	}
	if (m_Poly)
		m_Poly->SetPos(Points[m_Current]);

	//-- 決定キー --
	if (KeyInput::GetKeyPush(VK_RETURN))
	{
		switch (m_Current)
		{
		case 0:
			SceneManager::GetInstance().SetNextChange(SceneType::Game);
			break;
		case 1:
			node::Application::SetGameFlag(false);
			break;
		default:
			break;
		}
	}
	

}
