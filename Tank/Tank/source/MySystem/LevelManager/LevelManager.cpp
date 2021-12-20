//------------------------------
//	レベルエディット管理
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#define _CRT_SECURE_NO_WARNINGS
#include "LevelManager.h"
#include "IMGUI\GUI_Message.h"
#include "Scene\SceneManager.h"
#include "Component\Edit\EditPanel.h"
#include "MySystem\LevelManager\MapManager.h"

namespace
{
	const std::string GameLevelDataFileName = "data/LevelData/LevelSaveData.lsd";
	const std::string PanelDataPath = "data/LevelData/PanelData/";
}

//-- 静的メンバ --
std::map<std::string, LevelManager::LevelInfo> LevelManager::GameLevelData;
std::vector<LevelManager::PanelInfo> LevelManager::CurrentLoadData;

/*
	初期化
	パラメータ無し
*/
void LevelManager::Init()
{
	//-- ファイル読み込み --
	FILE* fp = fopen(GameLevelDataFileName.c_str(), "rb");
	if (!fp) return;

	GameLevelData.clear();
	uint16_t size = 0;
	fread(&size, sizeof(uint16_t), 1, fp);

	for(uint16_t i = 0;i < size;i++)
	{
		char buf[256]{};
		fread(&buf, sizeof(buf), 1, fp);
		std::string str = buf;
		LevelManager::LevelInfo info;
		fread(&buf, sizeof(buf), 1, fp);
		info.fileName = buf;
		fread(&info.ClearState, sizeof(bool), 1, fp);
		fread(&buf, sizeof(buf), 1, fp);
		info.NextStageName = buf;
		GameLevelData.insert(std::make_pair(str, info));
	}

	fclose(fp);
	
}

/*
	終了処理
	パラメータ無し
*/
void LevelManager::Uninit()
{
	//-- ファイル保存 --
	FILE* fp = fopen(GameLevelDataFileName.c_str(), "wb");
	if (!fp) return;

	uint16_t size = static_cast<uint16_t>(GameLevelData.size());
	fwrite(&size, sizeof(uint16_t), 1, fp);
	for (auto obj : GameLevelData)
	{
		char buf[256]{};
		strcpy(buf, obj.first.c_str());
		fwrite(buf, sizeof(buf), 1, fp);
		strcpy(buf, obj.second.fileName.c_str());
		fwrite(buf, sizeof(buf), 1, fp);
		fwrite(&obj.second.ClearState, sizeof(bool), 1, fp);
		strcpy(buf, obj.second.NextStageName.c_str());
		fwrite(buf, sizeof(buf), 1, fp);
	}

	fclose(fp);
}

/*
	レベルデータセーブ
	引数 : レベル名,レベル情報
*/
void LevelManager::SaveLevelData(std::string LevelName, LevelInfo info)
{
	//-- 現在のシーンからパネル情報を取得 --
	CurrentLoadData.clear();
	auto manager = SceneManager::GetInstance().GetCurrentScene()->manager;
	auto panels = manager->FindObjectsWithTag("Panel");
	int playerCount = 0;
	for (auto obj : panels)
	{
		auto panel = obj->GetComponent<EditPanel>();
		PanelInfo info;
		info.coord = panel->GetCoord();
		info.type = panel->GetType();
		if (info.type == PanelType::Player)
			playerCount += 1;
		CurrentLoadData.push_back(info);
	}
	//-- プレイヤー数チェック --
	if (playerCount != 1)
	{
		IG::MessageManager::DrawSystemLog(u8"プレイヤーが規定の数ではありません。");
		return;
	}

	//-- レベルデータへの登録 --
	if (GameLevelData.find(LevelName) == GameLevelData.end())
	{
		GameLevelData.insert(std::make_pair(LevelName, info));
	}
	else
	{
		GameLevelData[LevelName] = info;
	}


	//-- ファイルに書き出し --
	std::string filename = PanelDataPath;
	filename += LevelName;
	filename += ".lvl";
	FILE* fp = fopen(filename.c_str(), "wb");

	uint16_t size = static_cast<uint16_t>(CurrentLoadData.size());
	fwrite(&size, sizeof(uint16_t), 1, fp);
	for (auto obj : CurrentLoadData)
	{
		fwrite(&obj.type, sizeof(obj.type), 1, fp);
		fwrite(&obj.coord, sizeof(obj.coord), 1, fp);
	}

	fclose(fp);

	IG::MessageManager::DrawSystemLog("LevelDataSave : " + LevelName);
}

/*
	レベルデータロード
	引数 : レベル名
*/
void LevelManager::LoadLevelData(std::string LevelName)
{
	
	//-- セーブデータから読み込み --
	std::string filename = PanelDataPath;
	filename += LevelName;
	filename += ".lvl";
	FILE* fp = fopen(filename.c_str(), "rb");
	if (!fp) return;
	CurrentLoadData.clear();

	uint16_t size = 0;
	fread(&size, sizeof(uint16_t), 1, fp);

	for (uint16_t i = 0; i < size; i++)
	{
		PanelInfo info;
		fread(&info.type, sizeof(info.type), 1, fp);
		fread(&info.coord, sizeof(info.coord), 1, fp);
		CurrentLoadData.push_back(info);
	}

	fclose(fp);

	IG::MessageManager::DrawSystemLog("LevelDataLoad : " + LevelName);
}

/*
	レベルデータの削除
	引数 : レベル名
*/
void LevelManager::DeleteLevelData(std::string LevelName)
{
	for (auto it = GameLevelData.begin();it != GameLevelData.end();it++)
	{
		if (it->first == LevelName)
		{
			GameLevelData.erase(it);
			break;
		}
	}

	IG::MessageManager::DrawSystemLog("LevelDataDelete : " + LevelName);
}


/*
	現在データをエディターに適応
	パラメータ無し
*/
void LevelManager::AttachToEditor()
{
	//-- 現在のパネルを置き換え --
	{
		auto manager = SceneManager::GetInstance().GetCurrentScene()->manager;
		auto panels = manager->FindObjectsWithTag("Panel");
		
		for (auto panel : panels)
		{
			auto pane = panel->GetComponent<EditPanel>();
			auto coord = pane->GetCoord();
			for (auto obj : CurrentLoadData)
			{
				if (coord.x == obj.coord.x && coord.y == obj.coord.y)
				{
					pane->SetType(obj.type);
				}
			}
		}
	}
	
	IG::MessageManager::DrawSystemLog("AttachLevelData");
}

/*
	現在データを利用してゲーム画面へ
	パラメータ無し
*/
void LevelManager::AttachToMap()
{
	//todo
}

