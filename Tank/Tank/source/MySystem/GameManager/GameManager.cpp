//------------------------------
//	ゲーム進行管理
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "GameManager.h"
#include "MySystem\LevelManager\LevelManager.h"
#include "Scene\SceneManager.h"

namespace
{
	std::string FirstStageName = "Stage-1";
}

//-- 静的メンバ --
std::string GameManager::m_CurrentSelect = "default";
bool GameManager::isEnd = false;

/*
	初期化
	パラメータ無し
*/
void GameManager::Init()
{
	SetLevel(FirstStageName);	//初期
}

/*
	レベル名設定
	引数 : レベル名
*/
void GameManager::SetLevel(std::string LevelName)
{
	m_CurrentSelect = LevelName;
}

/*
	レベル生成
	パラメータ無し
*/
void GameManager::CreateLevel()
{
	LevelManager::LoadLevelData(m_CurrentSelect);
	LevelManager::AttachToMap();	//マップ情報アタッチ
}

/*
	ゲーム終了時の遷移
	パラメータ無し
*/
void GameManager::ChangeNextScene()
{
	auto next = LevelManager::GetNextLevelName(m_CurrentSelect);
	if (next.empty())
	{
		SceneManager::GetInstance().SetNextChange(SceneType::Title);
		m_CurrentSelect = FirstStageName;
	}
	else
	{
		m_CurrentSelect = next;
		SceneManager::GetInstance().SetNextChange(SceneType::Game);
	}
}

