//------------------------------
//	ロードシーンクラス
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "SceneBase.h"
#include <atomic>

//-- クラス定義 --
class SceneLoad final : public SceneBase
{
public:
	SceneLoad();
	~SceneLoad() = default;

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

	//-- 各種ロードメソッド --
private:
	void LoadTexture();
	void LoadModel();
	void LoadSprite();
	void LoadEffect();
private:
	std::atomic<int> m_LoadProgress;	//進行状態
	int m_LoadProcess;	//総ロード処理
	int m_CurrentCount;
	const int MinCount = 0;	//最低でもロード画面を見せるフレーム数
};

