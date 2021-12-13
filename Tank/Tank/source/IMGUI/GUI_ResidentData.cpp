//---------------------------
// IMGUI - 常駐データ管理
//-- author --
//	HatoriMasashi
//---------------------------

//-- include --
#include "GUI_ResidentData.h"
using namespace IG;
#include "MySystem\main.h"
#include "MySystem\Resident\ResidentData.h"
#include "Scene\SceneManager.h"

//-- 静的メンバ --
ResidentDataGUI* ResidentDataGUI::m_ResidentDataManager = nullptr;

/*
	GUI作成
	戻り値 : GUIへのポインタ
*/
ResidentDataGUI* ResidentDataGUI::Create()
{
	if (m_ResidentDataManager) return nullptr;

	m_ResidentDataManager = new ResidentDataGUI();
	m_ResidentDataManager->Init();

	return m_ResidentDataManager;
}

/*
	GUI削除
	パラメータ無し
*/
void ResidentDataGUI::Destroy()
{
	if (!m_ResidentDataManager) return;

	m_ResidentDataManager->Uninit();
	delete m_ResidentDataManager;
	m_ResidentDataManager = nullptr;
}

/*
	GUI初期化
	パラメータ無し
*/
void ResidentDataGUI::Init()
{
	m_Active = false;
}

/*
	GUI更新
	パラメータ無し
*/
void ResidentDataGUI::Update()
{
	//none
}

/*
	GUI描画
	パラメータ無し
*/
void ResidentDataGUI::Draw()
{
	if (!m_Active) return;

	//-- ウィンドウ設定 --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f,0.65f,0.1f,1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(150.0f, 150.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f), ImGuiCond_Once);

	//-- 描画開始 --
	ImGui::Begin("ResidentData", &m_Active);

	static ResidentData s_data = ResidentDataManager::GetData();

	if (ImGui::Button("SceneReload", { 100.0f,30.0f }))
	{
		SceneManager::GetInstance().SetNextChange(SceneManager::GetInstance().GetCurrentSceneType());
	}
	ImGui::SameLine();
	if (ImGui::Button("Save", { 50.0f,30.0f }))
	{
		ResidentDataManager::Save();
	}
	ImGui::Separator();

	ImGui::InputFloat("PlayerMaxSpeed", &s_data.PlayerData.MaxSpeed);
	ImGui::InputFloat("PlayerAccFrame", &s_data.PlayerData.AccFrame);
	ImGui::InputFloat("PlayerDecFrame", &s_data.PlayerData.DecFrame);
	ImGui::InputInt("ShotRate", &s_data.PlayerData.ShotRate);
	ImGui::InputInt("ShotMax", &s_data.PlayerData.ShotMax);

	ImGui::Separator();

	ImGui::InputFloat("EnemySpeed", &s_data.EnemyData.Speed);
	ImGui::InputInt(u8"EnemyShotRate", &s_data.EnemyData.ShotRate);

	ImGui::Separator();

	ResidentDataManager::SetData(s_data);	//データ再セット

	//-- 描画終了 --
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

/*
	GUI終了
	パラメータ無し
*/
void ResidentDataGUI::Uninit()
{
	//none
}
