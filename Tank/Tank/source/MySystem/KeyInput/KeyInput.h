//--------------------------
//	キー入力管理
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include <Windows.h>
#include <windef.h>

//-- define --
#define KEY_BIND_NUM	(256)
#define KEY_REPEAT_COUNT	(20)

//-- クラス定義 --
class KeyInput
{
public:
	static void Init();
	static void Update();
	static void Uninit();

	static bool GetKeyPress(int);
	static bool GetKeyPush(int);
	static bool GetKeyRelease(int);
	static bool GetKeyRepeat(int);
	static POINT GetMouseMove() { return m_MouseMove; }
	static POINT GetMousePos() { return m_CurrentMousePos; }
	static void SetMouseState(bool state);
	static void SetMousePos(POINT point);
private:
	static bool m_CurrentKeyState[KEY_BIND_NUM];
	static bool m_PrevKeyState[KEY_BIND_NUM];
	static int m_KeyCount[KEY_BIND_NUM];
	static POINT m_CurrentMousePos;		//現在のマウス座標
	static POINT m_PrevMousePos;		//マウスの過去座標
	static POINT m_MouseMove;			//マウス移動量
	static bool m_MouseState;			//マウスの状態
};



