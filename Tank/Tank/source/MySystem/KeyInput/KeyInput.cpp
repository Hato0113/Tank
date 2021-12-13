//--------------------------
//	キー入力管理
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "MySystem\main.h"
#include "KeyInput.h"
#include <Windows.h>

//-- 静的メンバ --
bool KeyInput::m_CurrentKeyState[KEY_BIND_NUM] = { false };
bool KeyInput::m_PrevKeyState[KEY_BIND_NUM] = { false };
int KeyInput::m_KeyCount[KEY_BIND_NUM] = { 0 };
POINT KeyInput::m_CurrentMousePos = { 0,0 };
POINT KeyInput::m_PrevMousePos = { 0,0 };
POINT KeyInput::m_MouseMove = { 0,0 };
bool KeyInput::m_MouseState = true;

void KeyInput::Init()
{
	//-- キー情報初期化 --
	for (int i = 0; i < KEY_BIND_NUM; i++)
	{
		KeyInput::m_CurrentKeyState[i] = false;
		KeyInput::m_PrevKeyState[i] = false;
		KeyInput::m_KeyCount[i] = 0;
	}
	//-- マウス座標初期化 --
	//GetCursorPos(&m_CurrentMousePos);
	m_PrevMousePos = m_CurrentMousePos;
	m_MouseMove = { 0,0 };
	m_MouseState = true;
}

void KeyInput::Update()
{
	//-- キーボード更新 --
	for (int i = 0; i < KEY_BIND_NUM; i++)
	{
		//-- 1F前の情報に --
		m_PrevKeyState[i] = m_CurrentKeyState[i];
		if (GetAsyncKeyState(i))
		{
			m_CurrentKeyState[i] = true;
			m_KeyCount[i]++;
		}
		else
		{
			m_CurrentKeyState[i] = false;
			if (m_KeyCount[i])
				m_KeyCount[i] = 0;
		}
	}

	//-- マウス座標更新 --
	//GetCursorPos(&m_CurrentMousePos);
	//-- Windowの座標を加算 --
	//RECT rect;
	//GetWindowRect(DX::DirectXManager::GetInstance().GetWndHandle(), &rect);
	//m_CurrentMousePos.x -= rect.left;
	//m_CurrentMousePos.y -= rect.top;



	m_MouseMove.x = m_CurrentMousePos.x - m_PrevMousePos.x;
	m_MouseMove.y = m_CurrentMousePos.y - m_PrevMousePos.y;
	m_PrevMousePos = m_CurrentMousePos;
}

void KeyInput::Uninit()
{
	ShowCursor(false);
}

bool KeyInput::GetKeyPush(int code)
{
	if (m_CurrentKeyState[code] == true &&
		m_PrevKeyState[code] == false)
	{
		return true;
	}
	else return false;
}

bool KeyInput::GetKeyPress(int code)
{
	if (m_CurrentKeyState[code] == true) return true;
	else return false;
}

bool KeyInput::GetKeyRelease(int code)
{
	if (m_CurrentKeyState[code] == false &&
		m_PrevKeyState[code] == true)
	{
		return true;
	}
	else return false;
}

bool KeyInput::GetKeyRepeat(int code)
{
	if (GetKeyPush(code)) return true;

	if (m_KeyCount[code] >= KEY_REPEAT_COUNT)
	{
		return true;
	}
	else return false;
}

void KeyInput::SetMouseState(bool state)
{
	m_MouseState = state;
}

void KeyInput::SetMousePos(POINT point)
{
	if (point.y > 4 * WindowInfo::m_ScreenHeight + WindowInfo::m_GUIWindowHeight)
		point.y = 0;
	if (point.x > 4 * WindowInfo::m_ScreenWidth + WindowInfo::m_GUIWindowWidth)
		point.x = 0;
	m_CurrentMousePos = point;
}

