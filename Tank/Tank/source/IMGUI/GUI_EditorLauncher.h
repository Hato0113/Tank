//----------------------------------
//	GUI_EditorLauncher.h
//	エディターランチャー
//	IMGUIのウィンドウを一括管理
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once
//-- include --
#include "MyImGui.h"
#include <map>

namespace IG
{
	class Launcher;
	class Editor;
}

/*
	IMGUI統合管理クラス
	ウィンドウの作成 / アクティブ状態の切り替え等を行う
*/

//-- クラス定義 --
/*
	IMGUIウィンドウ管理クラス
	静的メソッドのみを持つ
*/
class IG::Editor;
class IG::Launcher final
{
	Launcher() = delete;
public:
	static void Init();
	static void Update();
	static void Draw();
	static void Uninit();

	static std::map<std::string, IG::Editor*> m_WindowMap;
	static bool m_WindowFlag;
};

/*
	エディタークラス
	このクラスと基底としたウィンドウをLauncherクラスで管理する。
	一度登録されたものは[x]を押しても非表示になるものとする。
*/
class IG::Editor
{
public:
	Editor() {}
	virtual ~Editor() {}
public:
	virtual void Destroy() = 0;
	virtual void Init() = 0;	//初期化
	virtual void Uninit() = 0;	//終了処理
	virtual void Update() = 0;	//更新処理
	virtual void Draw() = 0;	//描画処理
	bool m_Active = true;
};


