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
#include "System\Sound.h"

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
	//-- α値変更 --
	m_Alpha += DirectX::XM_PI / 180.0f * 3;
	float al = (sinf(m_Alpha) + 1.0f) * 0.5f;
	m_Poly->SetAlpha(al);

	if (Selected) return;

	//-- 決定キー --
	if (KeyInput::GetKeyPush(VK_SPACE))
	{
		SceneManager::GetInstance().SetNextChange(SceneType::Game);
		CSound::Play(SE_Decision);
		Selected = true;
	}

	//-- ステージエディットモード --
	if (KeyInput::GetKeyPush(VK_F1))
	{
		SceneManager::GetInstance().SetNextChange(SceneType::Edit);
		CSound::Play(SE_Decision);
		Selected = true;
	}


}
