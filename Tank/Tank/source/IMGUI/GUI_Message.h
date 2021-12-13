//----------------------------------
//	GUI_Message.h
//	メッセージウィンドウクラス
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once
//-- include --
#include "MyImGui.h"
#include <list>
#include "GUI_EditorLauncher.h"
#include <vector>

namespace IG
{
	class Message;
	class MessageManager;
}

/*
	GUIメッセージクラス
	各オブジェクトから送られたメッセージをリスト管理
	メッセージはID管理し、更新関数が呼ばれている間は同一メッセージとして扱う
	UnityのDebug.Logを参考に。
*/

//-- クラス定義 --
/*
	メッセージオブジェクトクラス
	これ一つをメッセージとして保存
*/
class IG::Message
{
	friend class IG::MessageManager;
private:
	char m_szMessage[256];
	int m_LifeTime;
	int m_MessageLen;
	char m_szOriginalMessage[256];
};

/*
	メッセージ管理クラス
	自身のポインタを静的メンバとして持つことで
	疑似シングルトンオブジェクトとして生成
*/
class IG::MessageManager : public Editor
{
private:
	MessageManager() {}
public:
	static void DrawLog(const char* message, ...);	//一定フレームで消える、データを含むメッセージ
	static void DrawSystemLog(std::string message);	//消えない単一メッセージ
	static MessageManager* Create();
	void Destroy() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	std::list<Message*> m_MessageList;
	std::vector<std::string> m_SystemLog;
	static MessageManager* m_MessageManager;	//オブジェクトはこれのみ
	static bool CreateSameMessageFlag;
};


