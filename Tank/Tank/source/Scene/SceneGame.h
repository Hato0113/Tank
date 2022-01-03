//------------------------------
//	ゲームシーンクラス
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "SceneBase.h"

//-- クラス定義 --
class SceneGame final : public SceneBase
{
public:
	SceneGame();
	~SceneGame() = default;

	/*
		初期化
		パラメータ無し
	*/
	void Init() override;

	/*
		更新
		パラメータ無し
	*/
	void Update() override;
	/*
		描画
		パラメータ無し
	*/
	void Draw() override;
	/*
		終了処理
		パラメータ無し
	*/
	void Uninit() override;

private:
	/*
		テストステージ生成
		パラメータ無し
	*/
	void SummonStage();
};





