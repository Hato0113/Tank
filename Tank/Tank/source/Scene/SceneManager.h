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
	static SceneManager& GetInstance()
	{
		static SceneManager inst;
		return inst;
	}

	void Update();
	void Draw();
	void Uninit();

	void Add(SceneType, SceneBase*);
	void SetNextChange(SceneType);
	void SetFirst(SceneType);
	SceneBase* GetCurrentScene() { return m_pCurrentScene; }
	SceneType GetCurrentSceneType() { return m_CurrentType; }

private:
	std::unordered_map<SceneType, SceneBase*> m_SceneMap;
	SceneBase* m_pCurrentScene;
	SceneBase* m_pNextScene;
	Fade* m_Fade;
	SceneType m_CurrentType;
};