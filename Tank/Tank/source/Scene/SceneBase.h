/**
 * @brief シーンインターフェース
 * @author HatoriMasashi
 */
#pragma once

//-- クラス定義 --
/**
 * @brief シーンインターフェース
 */
class ObjectManager;
class SceneBase abstract
{
public:
	/**
	 * @brief コンストラクタ
	 */
	SceneBase();

	/**
	 * @brief デストラクタ
	 */
	virtual ~SceneBase() = default;

	/**
	 * @brief 初期化
	 */
	virtual void Init();
	
	/**
	 * @brief 更新
	 */
	virtual void Update();
	
	/**
	 * @brief 描画
	 */
	virtual void Draw();
	
	/**
	 * @brief 終了処理
	 */
	virtual void Uninit();

public:
	//! オブジェクトマネージャ
	ObjectManager* manager;
};

#define ISCENE_H
//-- include --
#include "Objects\ObjectManager.h"


