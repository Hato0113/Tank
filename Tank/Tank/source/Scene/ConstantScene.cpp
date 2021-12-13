//------------------------------
//	常駐シーン
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "ConstantScene.h"
#include "Allocator/Allocator.hpp"
#include "MySystem\KeyInput\KeyInput.h"
#include "Component\Mouse\MouseInfo.h"
#include "MySystem\Collider\ColliderManager.h"
#include "Component\Fade\Fade.h"
#include "Component\Debug\DebugPause.h"
#include "MySystem\Effect\EffectManager.h"


ConstantScene::ConstantScene()
{
	manager = nullptr;
}

void ConstantScene::Init()
{
	//-- サブシステム初期化 --
	manager = Allocator::Alloc<ObjectManager>();
	manager->m_pScene = this;

	KeyInput::Init();
	ColliderManager::GetInstance().Init();

	//-- マウスマネージャ --
	if(true){
		auto obj = Object::Create("MouseManager");
		obj->AddComponent<MouseInfo>();
		manager->Add(obj);
	}

	//-- フェード --
	{
		auto obj = Object::Create("FadeCover");
		obj->AddComponent<Fade>();
		manager->Add(obj);
	}

	//-- debug ポーズ --
	{
		auto obj = Object::Create("DebugPause");
		obj->AddComponent<DebugPause>();
		manager->Add(obj);
	}
}

void ConstantScene::Update()
{
	KeyInput::Update();
	ColliderManager::GetInstance().Update();

	SceneBase::Update();
	EffectManager::Update();
}

void ConstantScene::Draw()
{
	SceneBase::Draw();
	EffectManager::Draw();
}

void ConstantScene::Uninit()
{
	EffectManager::Uninit();
	KeyInput::Uninit();
	ColliderManager::GetInstance().Uninit();
	Allocator::Free(manager);
}