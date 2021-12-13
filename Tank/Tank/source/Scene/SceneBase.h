//------------------------------
//	シーンインターフェースクラス
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- クラス定義 --
class ObjectManager;
class SceneBase abstract
{
public:
	SceneBase();
	virtual ~SceneBase() = default;

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Uninit();

	//-- メンバ変数 --
public:
	ObjectManager* manager;
};

#define ISCENE_H
//-- include --
#include "Objects\ObjectManager.h"


