//--------------------------
//	常駐するデータ
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#define _CRT_SECURE_NO_WARNINGS
#include "ResidentData.h"
#include "IMGUI\GUI_Message.h"

//-- 静的メンバ --
ResidentData ResidentDataManager::m_Data;

namespace
{
	std::string SaveFileName = "data/Status/ResidentData.resi";
}

/*
	初期化
	パラメータ無し
*/
void ResidentDataManager::Init()
{
	Load();
}

/*
	終了処理
	パラメータ無し
*/
void ResidentDataManager::Uninit()
{

}

/*
	ファイルロード
	引数 : ファイル名 (.resi)
*/
void ResidentDataManager::Load()
{
	FILE* fp = fopen(SaveFileName.c_str(), "rb");
	if (!fp) return;

	fread(&m_Data, sizeof(ResidentData), 1, fp);

	fclose(fp);

	IG::MessageManager::DrawSystemLog("LoadResidentData");
}

/*
	ファイル書き出し
	引数 : ファイル名 (.resi)
*/
void ResidentDataManager::Save()
{
	FILE* fp = fopen(SaveFileName.c_str(), "wb");
	if (!fp) return;

	fwrite(&m_Data, sizeof(ResidentData), 1, fp);

	fclose(fp);

	IG::MessageManager::DrawSystemLog("SaveResidentData");
}

