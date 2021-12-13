//------------------------------
//	タイトルシーンクラス
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "SceneBase.h"

//-- クラス定義 --
class SceneTitle final : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle() = default;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
};





