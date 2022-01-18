/**
 * @brief ロードシーンクラス
 * @author HatoriMasashi
 */
#pragma once

//-- include --
#include "SceneBase.h"
#include <atomic>

//-- クラス定義 --
/**
 * @brief ロードシーン
 */
class SceneLoad final : public SceneBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	SceneLoad();

	/**
	 * @brief デストラクタ
	 */
	~SceneLoad() = default;

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

	//-- 各種ロードメソッド --
private:
	/**
	 * @brief テクスチャロード
	 */
	void LoadTexture();

	/**
	 * @brief モデルロード
	 */
	void LoadModel();

	/**
	 * @brief スプライトロード
	 */
	void LoadSprite();

	/**
	 * @brief エフェクトロード
	 */
	void LoadEffect();
private:
	//! 進行状況
	std::atomic<int> m_LoadProgress;
	//! 総ロード処理
	int m_LoadProcess;
	//! 現在ロード数
	int m_CurrentCount;
	//! 最小フレーム数
	const int MinCount = 120;
	//! スレッドの有効状態
	std::atomic<bool> m_ThreadEnable;
};

