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

	/*
		初期化
		パラメータ無し
	*/
	virtual void Init();
	/*
		更新
		パラメータ無し
	*/
	virtual void Update();
	/*
		描画
		パラメータ無し
	*/
	virtual void Draw();
	/*
		終了処理
		パラメータ無し
	*/
	virtual void Uninit();

	//-- メンバ変数 --
public:
	ObjectManager* manager;	//オブジェクトマネージャ
};

#define ISCENE_H
//-- include --
#include "Objects\ObjectManager.h"


