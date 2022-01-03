//------------------------------
//	ゲーム進行管理
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include <string>

//-- クラス定義 --
class GameManager
{
public:
	static void Init();
	static void SetLevel(std::string LevelName);	//選択レベル変更
	static void CreateLevel();						//レベルをオブジェクトとして生成
	static std::string GetLevelName() { return m_CurrentSelect; }
public:
	static std::string m_CurrentSelect;		//現在選択されているレベル名
};

