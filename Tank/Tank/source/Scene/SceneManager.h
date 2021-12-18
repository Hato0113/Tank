//------------------------------
//	シーンマネージャークラス
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "SceneBase.h"
#include <unordered_map>
#include "Component\Fade\Fade.h"


//-- 列挙 --
enum class SceneType
{
	Title = 0,
	Game,
	Load,

	TypeMax,
};

//-- クラス定義 --
class SceneManager final
{
private:
	SceneManager();
	~SceneManager() {};
public:
	/*
		インスタンス取得関数
		戻り値 : インスタンスへの参照
	*/
	static SceneManager& GetInstance()
	{
		static SceneManager inst;
		return inst;
	}

	/*
		更新
		パラメータ無し
	*/
	void Update();
	/*
		描画
		パラメータ無し
	*/
	void Draw();
	/*
		終了処理
		パラメータ無し
	*/
	void Uninit();

	/*
		シーン情報追加
		引数 : シーンタイプ,シーンオブジェクトへのポインタ
	*/
	void Add(SceneType, SceneBase*);

	/*
		シーン移動
		引数 : シーンタイプ
	*/
	void SetNextChange(SceneType);

	/*
		初期シーン設定
		引数 : シーンタイプ
	*/
	void SetFirst(SceneType);

	/*
		現在シーン取得
		戻り値 : シーンオブジェクトへのポインタ
	*/
	SceneBase* GetCurrentScene() { return m_pCurrentScene; }
	/*
		現在シーンタイプ取得
		戻り値 : シーンタイプ
	*/
	SceneType GetCurrentSceneType() { return m_CurrentType; }

private:
	std::unordered_map<SceneType, SceneBase*> m_SceneMap;	//シーン管理マップ
	SceneBase* m_pCurrentScene;	//現在シーン
	SceneBase* m_pNextScene;	//次のシーン
	Fade* m_Fade;	//フェードオブジェクトへのポインタ
	SceneType m_CurrentType;	//現在シーンタイプ
};