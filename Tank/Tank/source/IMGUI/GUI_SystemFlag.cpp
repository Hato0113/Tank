//---------------------------
// IMGUI - 常駐データ管理
//-- author --
//	HatoriMasashi
//---------------------------

//-- include --
#include "GUI_SystemFlag.h"
using namespace IG;
#include "MySystem\main.h"
#include "MySystem\Resident\ResidentFlag.h"
#include "Scene\SceneManager.h"
#include "GUI_Message.h"

//-- 静的メンバ --
ResidentFlagGUI* ResidentFlagGUI::m_ResidentFlagManager = nullptr;

/*
	GUI作成
	戻り値 : GUIへのポインタ
*/
ResidentFlagGUI* ResidentFlagGUI::Create()
{
	if (m_ResidentFlagManager) return nullptr;

	m_ResidentFlagManager = new ResidentFlagGUI();
	m_ResidentFlagManager->Init();

	return m_ResidentFlagManager;
}

/*
	GUI削除
	パラメータ無し
*/
void ResidentFlagGUI::Destroy()
{
	if (!m_ResidentFlagManager) return;

	m_ResidentFlagManager->Uninit();
	delete m_ResidentFlagManager;
	m_ResidentFlagManager = nullptr;
}

/*
	GUI初期化
	パラメータ無し
*/
void ResidentFlagGUI::Init()
{
	m_Active = false;
}

/*
	GUI更新
	パラメータ無し
*/
void ResidentFlagGUI::Update()
{
	//none
}

/*
	GUI描画
	パラメータ無し
*/
void ResidentFlagGUI::Draw()
{
	if (!m_Active) return;

	//-- ウィンドウ設定 --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.1f, 0.65f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f / 2, 0.65f / 2, 1.0f / 2, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f), ImGuiCond_Once);

	//-- 描画開始 --
	ImGui::Begin("ResidentFlag", &m_Active);

	static ResidentFlag s_flag = ResidentFlagManager::GetData();
	ImGui::Text("SystemFlag");
	if (ImGui::Checkbox("Pause", &s_flag.SystemFlag.Pause))
	{
		if (s_flag.SystemFlag.Pause) IG::MessageManager::DrawSystemLog("Pause");
		else IG::MessageManager::DrawSystemLog("Play");
	}
	if (ImGui::Checkbox("ShowCollider", &s_flag.SystemFlag.ShowCollider))
	{
		if (s_flag.SystemFlag.ShowCollider) IG::MessageManager::DrawSystemLog("ShowCollider");
		else IG::MessageManager::DrawSystemLog("Don't Show");
	}
	ImGui::Separator();

	ImGui::Text("GamePlay");
	if (ImGui::Checkbox("Player : Invincible", &s_flag.GamePlay.Player.Invincible))
	{
		if (s_flag.GamePlay.Player.Invincible) IG::MessageManager::DrawSystemLog("Invincible true");
		else IG::MessageManager::DrawSystemLog("Invincible false");
	}
	if (ImGui::Checkbox("Player : AutoShot", &s_flag.GamePlay.Player.AutoShot))
	{
		if (s_flag.GamePlay.Player.AutoShot) IG::MessageManager::DrawSystemLog("AutoShot true");
		else IG::MessageManager::DrawSystemLog("AutoShot false");
	}
	if (ImGui::Checkbox("Player : InfiniteBullet", &s_flag.GamePlay.Player.InfiniteBullet))
	{
		if (s_flag.GamePlay.Player.InfiniteBullet) IG::MessageManager::DrawSystemLog("InfiniteBullet true");
		else IG::MessageManager::DrawSystemLog("InfiniteBullet false");
	}
	if (ImGui::Checkbox("Enemy : Move", &s_flag.GamePlay.Enemy.Move))
	{
		if (s_flag.GamePlay.Enemy.Move) IG::MessageManager::DrawSystemLog("EnemyMove true");
		else IG::MessageManager::DrawSystemLog("EnemyMove false");
	}
	if (ImGui::Checkbox("Enemy : Shot", &s_flag.GamePlay.Enemy.Shot))
	{
		if (s_flag.GamePlay.Enemy.Shot) IG::MessageManager::DrawSystemLog("EnemyShot true");
		else IG::MessageManager::DrawSystemLog("EnemyShot false");
	}

	//-- データ再セット --
	ResidentFlagManager::SetData(s_flag);

	

	//-- 描画終了 --
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

/*
	GUI終了
	パラメータ無し
*/
void ResidentFlagGUI::Uninit()
{
	//none
}
