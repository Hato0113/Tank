//----------------------------------
//	GUI_Message.cpp
//	メッセージウィンドウクラス
//-- author --
//	HatoriMasahi
//-- Log --
//	2021/06/14 作成
//----------------------------------

//-- include --
#define _CRT_SECURE_NO_WARNINGS
#include "GUI_Message.h"
#include "MySystem\main.h"
using namespace IG;

//-- 静的メンバ --
MessageManager* MessageManager::m_MessageManager = nullptr;
bool MessageManager::CreateSameMessageFlag = false;

/*
	ログセット関数
	表示するメッセージの設定を行う
*/
void MessageManager::DrawLog(const char* message, ...)
{
	if (m_MessageManager == nullptr) return;

	//-- メッセージ解析 --
	va_list list;
	va_start(list, message);
	char aBuf[256];
	_vsprintf_p(aBuf, sizeof(aBuf), message, list);
	va_end(list);
	int len = (int)strlen(message);

	for (auto temp : m_MessageManager->m_MessageList)
	{
		if (temp->m_MessageLen == len)
		{
			//-- 全く同じメッセージかを解析 --
			if (strcmp(message, temp->m_szOriginalMessage) == 0 && !CreateSameMessageFlag)
			{
				//-- 同じ場合はメッセージの作成はしない --
				temp->m_LifeTime = 180;		//180Fの間は生存
				//-- メッセージの更新 --
				strcpy(temp->m_szMessage, aBuf);
				return;
			}
		}
	}

	//-- 新しいメッセージとして登録 --
	Message* msg = new Message();
	msg->m_MessageLen = len;
	strcpy(msg->m_szMessage, aBuf);
	msg->m_LifeTime = 180;	//生存時間設定
	strcpy(msg->m_szOriginalMessage, message);
	m_MessageManager->m_MessageList.push_back(msg);	//リスト登録
}

/*
	システムログ追加
	引数 : ログメッセージ
*/
void MessageManager::DrawSystemLog(std::string str)
{
	if (m_MessageManager == nullptr) return;
	m_MessageManager->m_SystemLog.push_back(str);
}


/*
	ウィンドウ作成関数
*/
MessageManager* MessageManager::Create()
{
	if (m_MessageManager != nullptr) return nullptr;

	m_MessageManager = new MessageManager();
	m_MessageManager->Init();

	return m_MessageManager;
}

/*
	ウィンドウ削除
*/
void MessageManager::Destroy()
{
	if (m_MessageManager == nullptr) return;

	m_MessageManager->Uninit();
	delete m_MessageManager;
	m_MessageManager = nullptr;
}

/*
	初期化関数
*/
void MessageManager::Init()
{
	//-- メンバ変数初期化 --
	m_MessageList.clear();
}

/*
	更新関数
*/
void MessageManager::Update()
{
	for (auto itr = m_MessageList.begin(); itr != m_MessageList.end(); )
	{
		if ((*itr)->m_LifeTime-- == 0)	//生存時間切れ
			itr = m_MessageList.erase(itr);
		else
			itr++;
	}
}

/*
	描画関数
*/
void MessageManager::Draw()
{
	if (!m_Active) return;
	//-- ウィンドウ設定 --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.7f / 2, 0.2f / 2, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(0.0f, (float)WindowInfo::m_ScreenHeight),ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)WindowInfo::m_ScreenWidth / 2, (float)WindowInfo::m_GUIWindowHeight), ImGuiCond_Once);

	ImGui::Begin("DebugMessage", &m_Active);

	for (auto it : m_MessageList)
	{
		ImGui::Text(it->m_szMessage);
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//-- 単一メッセージ描画用 --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.078f, 0.576f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(1.0f / 2, 0.078f / 2, 0.576f / 2, 1.0f));
	ImGui::SetNextWindowPos(ImVec2((float)WindowInfo::m_ScreenWidth / 2, (float)WindowInfo::m_ScreenHeight), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)WindowInfo::m_ScreenWidth / 2, (float)WindowInfo::m_GUIWindowHeight), ImGuiCond_Once);

	ImGui::Begin("SystemLog", &m_Active);

	if (ImGui::Button("LogDelete", { 100.0f,30.0f }))
	{
		m_SystemLog.clear();
	}
	ImGui::Separator();

	for (auto log : m_SystemLog)
	{
		ImGui::Text(log.c_str());
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

/*
	終了処理
*/
void MessageManager::Uninit()
{
	//-- メッセージオブジェクト削除 --
	auto tempList = m_MessageList;
	for (auto it : tempList)
	{
		delete it;
	}

	//-- リスト初期化 --
	m_MessageList.clear();
}
