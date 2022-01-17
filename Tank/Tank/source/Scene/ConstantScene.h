/**
 * @brief 常駐シーン
 * @author HatoriMasashi
 */
#pragma once

//-- include --
#include "Scene\SceneBase.h"

//-- クラス定義 --
/**
 * 常駐シーン
 */
class ConstantScene final : public SceneBase
{
private:
	/**
	 * @brief コンストラクタ
	 */
	ConstantScene();

	/**
	 * @brief デストラクタ
	 */
	~ConstantScene() = default;

public:
	/**
	 * @brief 初期化
	 */
	void Init() override;

	/**
	 * @brief 更新
	 */
	void Update() override;
	
	/**
	 * @brief 更新
	 */
	void Draw() override;
	
	/**
	 * @brief 終了処理
	 */
	void Uninit() override;

	/**
	 * @brief インスタンス取得
	 * 
	 * @return インスタンス
	 */
	static ConstantScene& GetInstance()
	{
		static ConstantScene inst;
		return inst;
	}
};


