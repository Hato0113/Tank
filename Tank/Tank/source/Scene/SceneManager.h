/**
 * @brief シーンマネージャー
 * @author HatoriMasashi
 */
#pragma once

 //-- include --
#include "SceneBase.h"
#include <unordered_map>
#include "Component\Fade\Fade.h"

//-- enum --
/**
 * @brief シーンタイプ
 */
enum class SceneType
{
	Title = 0,
	Game,
	Load,
	Edit,

	TypeMax,
};

//-- クラス定義 --
/**
 * @brief シーンマネージャー
 */
class SceneManager final
{
private:
	/**
	 * @brief コンストラクタ
	 */
	SceneManager();

	/**
	 * @brief デストラクタ
	 */
	~SceneManager() = default;

public:
	/**
	 * @brief インスタンスゲット
	 *
	 * @return シーンマネージャーオブジェクト
	 */
	static SceneManager& GetInstance()
	{
		static SceneManager inst;
		return inst;
	}

	/**
	 * @brief 更新
	 */
	void Update();

	/**
	 * @brief 描画
	 */
	void Draw();

	/**
	 * @brief 終了処理
	 */
	void Uninit();

	/**
	 * @brief シーン情報追加
	 *
	 * @param シーンタイプ
	 * @param シーンオブジェクトポインタ
	 */
	void Add(SceneType, SceneBase*);

	/**
	 * @brief 次シーンの設定
	 *
	 * @param シーンタイプ
	 */
	void SetNextChange(SceneType);

	/**
	 * @brief 初期シーン設定
	 *
	 * @param シーンタイプ
	 */
	void SetFirst(SceneType);

	/**
	 * @brief 現在シーン取得
	 *
	 * @return シーンオブジェクト
	 */
	SceneBase* GetCurrentScene() { return m_pCurrentScene; }

	/**
	 * @brief 現在シーンタイプ取得
	 *
	 * @return シーンオブジェクト
	 */
	SceneType GetCurrentSceneType() { return m_CurrentType; }

private:
	//! シーン連想配列
	std::unordered_map<SceneType, SceneBase*> m_SceneMap;
	//! 現在シーン
	SceneBase* m_pCurrentScene;
	//! 次シーン
	SceneBase* m_pNextScene;
	//! フェードオブジェクトポインタ
	Fade* m_Fade;
	//! シーンタイプ
	SceneType m_CurrentType;
};