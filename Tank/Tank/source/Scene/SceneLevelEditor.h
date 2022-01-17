﻿/**
 * @brief レベルエディターシーン
 * @author HatoriMasashi
 */
#pragma once

//-- include --
#include "SceneBase.h"

//-- クラス定義 --
/**
 * @brief レベルエディターシーン
 */
class SceneLevelEditor final : public SceneBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	SceneLevelEditor();

	/**
	 * @brief デストラクタ
	 */
	~SceneLevelEditor() = default;

	/**
	 * @brief 初期化
	 */
	void Init() override;

	/**
	 * @brief 更新
	 */
	void Update() override;
	
	/**
	 * @brief 描画
	 */
	void Draw() override;
	
	/**
	 * @brief 終了処理
	 */
	void Uninit() override;
};





