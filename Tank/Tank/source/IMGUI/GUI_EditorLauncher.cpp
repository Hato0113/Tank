//----------------------------------
//	GUI_EditorLauncher.cpp
//	エディターランチャー
//	IMGUIのウィンドウを一括管理
//-- author --
//	HatoriMasahi
//-- Log --
//	2021/06/15 作成
//----------------------------------

//-- include --
#include "GUI_EditorLauncher.h"
#include "GUI_Message.h"
using namespace IG;
#include "../imgui/imgui.h"
#include "MySystem\main.h"
#include "MySystem\DirectX.h"
#include "MySystem\Application.hpp"
#include "GUI_CameraInfo.h"
#include "GUI_Hierarchy.h"
#include "GUI_ResidentData.h"
#include "GUI_SystemFlag.h"
#include "GUI_LevelEditor.h"
#include "Scene\SceneManager.h"

//-- 静的メンバ --
std::map<std::string, IG::Editor*> Launcher::m_WindowMap;
bool Launcher::m_WindowFlag = false;

/*
	エディター初期化関数
*/
void IG::Launcher::Init()
{
	//-- マップ初期化 --
	m_WindowMap.clear();
	//-- メッセージウィンドウ生成 --
	m_WindowMap.insert(std::make_pair("Message", MessageManager::Create()));
	//-- カメラ情報ウィンドウ生成 --
	m_WindowMap.insert(std::make_pair("CameraInfo", CameraInfo::Create()));
	//-- ヒエラルキービュー生成 --
	m_WindowMap.insert(std::make_pair("Hierarchy", Hierarchy::Create()));
	//-- 常駐データビュー生成 --
	m_WindowMap.insert(std::make_pair("ResidentData", ResidentDataGUI::Create()));
	//-- システムフラグビュー生成 --
	m_WindowMap.insert(std::make_pair("ResidentFlag", ResidentFlagGUI::Create()));
	//-- レベルエディター生成 --
	m_WindowMap.insert(std::make_pair("LevelEditor", LevelEditorGUI::Create()));
}

/*
	エディター更新関数
*/
void IG::Launcher::Update()
{
	for (auto it : m_WindowMap)
	{
		it.second->Update();
	}
}

/*
	エディター描画関数
*/
void IG::Launcher::Draw()
{
	//-- ランチャー描画 --
	ImGuiWindowFlags WindowFlags = 0;
	WindowFlags |= ImGuiWindowFlags_MenuBar;
	ImGui::SetNextWindowPos(ImVec2((float)WindowInfo::m_ScreenWidth, 10.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)WindowInfo::m_GUIWindowWidth, 200.0f), ImGuiCond_Once);

	ImGui::Begin("EditorLauncher", &m_WindowFlag, WindowFlags);
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	if(ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("System"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				tank::Application::SetGameFlag(false);
			}
			if (ImGui::MenuItem("TitleScene"))
			{
				SceneManager::GetInstance().SetNextChange(SceneType::Title);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("MessageWindow", NULL, &m_WindowMap.at("Message")->m_Active);
			ImGui::MenuItem("CameraInfo", NULL, &m_WindowMap.at("CameraInfo")->m_Active);
			ImGui::MenuItem("Hierarchy", NULL, &m_WindowMap.at("Hierarchy")->m_Active);
			ImGui::MenuItem("ResidentData", NULL, &m_WindowMap.at("ResidentData")->m_Active);
			ImGui::MenuItem("ResidentFlag", NULL, &m_WindowMap.at("ResidentFlag")->m_Active);
			ImGui::MenuItem("LevelEditor", NULL, &m_WindowMap.at("LevelEditor")->m_Active);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

		//-- デバッグコマンドリファレンス --
		ImGui::Text("F1 / UpdateStop");
		ImGui::Text("F2 / ShowCollider");
	}

	ImGui::End();

	//-- 各ウィンドウ描画 --
	for (auto it : m_WindowMap)
	{
		it.second->Draw();
	}

	//demo
	//ImGui::ShowDemoWindow();
}

/*
	エディター終了関数
*/
void IG::Launcher::Uninit()
{
	for (auto it : m_WindowMap) {
		it.second->Destroy();
	}
	m_WindowMap.clear();
}


