//------------------------------
//	シーンインターフェースクラス
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "SceneBase.h"

SceneBase::SceneBase()
{
	manager = nullptr;
}

void SceneBase::Init()
{
	//none
}

void SceneBase::Update()
{
	manager->Update();
}

void SceneBase::Draw()
{
	manager->Draw();
}

void SceneBase::Uninit()
{
	if (manager)
		Allocator::Free(manager);
}

