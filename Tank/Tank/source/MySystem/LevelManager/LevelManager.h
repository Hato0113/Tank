//------------------------------
//	レベルエディット管理
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <map>

//-- enum --
class EditPanel;
enum class PanelType : int
{
	None = 0,
	Player = 1,
	EnemyNormal = 2,
	EnemyQuickly = 3,
	EnemyRapidFire = 4,
	EnemyStrong = 5,
	Wall = 20,
	Niedle = 25,
};

//-- クラス定義 --
class LevelManager
{
	//-- 構造体定義 --
public:
	struct LevelInfo
	{
		std::string fileName;	//レベルデータの入っているファイル名
		bool ClearState;		//クリアフラグ
		std::string NextStageName;	//次ステージ名
	};
	struct PanelInfo
	{
		PanelType type;			//パネル情報
		DirectX::XMINT2 coord;	//パネルタイプ
	};

	//-- メソッド定義 --
public:
	static void Init();	//初期化
	static void Uninit();	//終了処理
	static void SaveLevelData(std::string LevelName, LevelInfo info);
	static void LoadLevelData(std::string LevelName);
	static void DeleteLevelData(std::string LevelName);
	static std::string GetNextLevelName(std::string levelName);
	static void ResetPanel();	//パネルリセット

public:
	static void AttachToMap();
	static void AttachToEditor();

	//-- 情報保存 --
public:
	static std::map<std::string, LevelInfo> GameLevelData;	//レベル全体のデータ
	static std::vector<PanelInfo> CurrentLoadData;	//現在ロードしているパネル情報
};
