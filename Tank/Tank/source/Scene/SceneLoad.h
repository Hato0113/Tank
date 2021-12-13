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

	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

private:
	void LoadTexture();
	void LoadModel();
	void LoadSprite();
	void LoadEffect();
private:
	std::atomic<int> m_LoadProgress;	//進行状態
	int m_LoadProcess;	//総ロード処理
	int m_CurrentCount;
	const int MinCount = 0;	//最低でも180Fはシーンを持たせる
};

