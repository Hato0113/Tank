//----------------------
//	main.h
//-- author --
//	HatoriMasashi
//----------------------
#pragma once

//-- include --
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

//-- クラス定義 --
class WindowInfo
{
public:
	static int m_ScreenHeight;
	static int m_ScreenWidth;
	static int m_GUIWindowHeight;
	static int m_GUIWindowWidth;
	static bool m_DebugGUIFlag;
};

//-- プロトタイプ宣言 --
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);