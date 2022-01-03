//-----------------------
//	ゲーム終了時TL
//-- author --
//	HatoriMasahi
//-----------------------
#pragma once

//-- include --
#include "GameEndTL.h"
#include "Component\Polygon\Polygon.h"
#include "MySystem\Texture\TextureManager.h"
#include "Scene\SceneManager.h"

GameEndTL::GameEndTL(bool state) : m_State(state)
{

}

void GameEndTL::Update()
{
	switch (TimelineBase::m_CurrentTime)
	{
	case 0:
	{	//ポリゴン生成
		auto obj = Object::Create("clear");
		auto poly = obj->AddComponent<CPolygon>();
		poly->SetSize({ 768.0f,256.0f });
		if (m_State)
			poly->SetTex(TextureManager::Get(TextureID::game_Clear));
		else
			poly->SetTex(TextureManager::Get(TextureID::game_Failed));
		parent->GetScene()->manager->Add(obj);
	}
		break;
	case 120:
	{	//シーン移動
		SceneManager::GetInstance().SetNextChange(SceneType::Title);
	}
		break;
	default:
		break;
	}
	TimelineBase::m_CurrentTime++;
}