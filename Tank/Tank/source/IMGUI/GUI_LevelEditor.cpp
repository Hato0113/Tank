//----------------------------
//	レベルエディター用GUI
//-- author --
//	HatoriMasashi
//----------------------------

//-- include --
#include "GUI_LevelEditor.h"
using namespace IG;
#include "MySystem\main.h"
#include "GUI_Message.h"
#include "MySystem\LevelManager\LevelManager.h"
#include "Scene\SceneManager.h"
#include "MySystem\GameManager\GameManager.h"

//-- 静的メンバ --
LevelEditorGUI* LevelEditorGUI::m_LevelEditor = nullptr;

/*
	GUI作成
	戻り値 : GUIへのポインタ
*/
LevelEditorGUI* LevelEditorGUI::Create()
{
	if (m_LevelEditor) return nullptr;

	m_LevelEditor = new LevelEditorGUI();
	m_LevelEditor->Init();

	return m_LevelEditor;
}

/*
	GUI削除
	パラメータ無し
*/
void LevelEditorGUI::Destroy()
{
	if (!m_LevelEditor) return;

	m_LevelEditor->Uninit();
	delete m_LevelEditor;
	m_LevelEditor = nullptr;
}

/*
	GUI初期化
	パラメータ無し
*/
void LevelEditorGUI::Init()
{
	m_Active = false;

	m_CurrentSelect = "none";
	m_PrevSelect = "none";
}

/*
	GUI更新
	パラメータ無し
*/
void LevelEditorGUI::Update()
{
	//none
}

/*
	GUI描画
	パラメータ無し
*/
void LevelEditorGUI::Draw()
{
	if (!m_Active) return;

	//-- ウィンドウ設定 --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.65f, 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(150.0f, 150.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f), ImGuiCond_Once);

	//-- 描画開始 --
	ImGui::Begin("LevelEdit", &m_Active);

	//-- レベル情報部分 --
	static char LevelName[128]{};
	ImGui::InputTextWithHint("LevelName", "LevelName", LevelName, IM_ARRAYSIZE(LevelName));
	static char NextStage[128]{};
	ImGui::InputTextWithHint("NextStage", "NextStageName", NextStage, IM_ARRAYSIZE(NextStage));
	if (ImGui::Button("Save"))
	{
		if (strcmp(LevelName, "") != 0)
		{
			//-- 情報のセーブ --
			LevelManager::LevelInfo info;
			info.fileName = LevelName;
			info.NextStageName = NextStage;
			info.ClearState = false;
			LevelManager::SaveLevelData(LevelName, info);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		LevelManager::DeleteLevelData(LevelName);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		LevelManager::ResetPanel();
	}
	
	if(ImGui::Button("AttachEditor"))
	{
		if (strcmp(LevelName, "") != 0)
		{
			LevelManager::AttachToEditor();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("AttachGame"))
	{
		if (strcmp(LevelName, "") != 0)
		{
			GameManager::SetLevel(LevelName);
			SceneManager::GetInstance().SetNextChange(SceneType::Game);
		}
	}

	ImGui::Separator();

	//-- レベルロード部分 --
	if (ImGui::BeginListBox("LevelDataList"))
	{
		for (auto obj : LevelManager::GameLevelData)
		{
			const bool is_selected = (m_CurrentSelect == obj.first);
			if (ImGui::Selectable(obj.first.c_str(), is_selected))
				m_CurrentSelect = obj.first;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	if (m_CurrentSelect != m_PrevSelect)
	{
		strcpy(LevelName, LevelManager::GameLevelData[m_CurrentSelect].fileName.c_str());
		strcpy(NextStage, LevelManager::GameLevelData[m_CurrentSelect].NextStageName.c_str());

		LevelManager::LoadLevelData(LevelName);

		m_CurrentSelect = m_PrevSelect;
	}

	//-- 描画終了 --
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

/*
	GUI終了
	パラメータ無し
*/
void LevelEditorGUI::Uninit()
{
	//none
}



