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

	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	void TestStage();
};





