//------------------------------
//	ゲーム進行管理
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "GameManager.h"
#include "MySystem\LevelManager\LevelManager.h"

//-- 静的メンバ --
std::string GameManager::m_CurrentSelect = "default";

/*
	初期化
	パラメータ無し
*/
void GameManager::Init()
{

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

