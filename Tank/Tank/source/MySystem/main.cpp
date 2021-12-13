//----------------------
//	main.cpp
//-- author --
//	HatoriMasashi
//----------------------

//-- include --
#include "main.h"
#include "DirectX.h"
#include <iostream>
#include "Application.hpp"
#include "KeyInput\KeyInput.h"

//-- 静的メンバ --
int WindowInfo::m_ScreenWidth = 1280;
int WindowInfo::m_ScreenHeight = 720;
int WindowInfo::m_GUIWindowWidth = 540;
int WindowInfo::m_GUIWindowHeight = 260;
bool WindowInfo::m_DebugGUIFlag = true;		//GUI使用フラグ

/*
	エントリポイント
	メイン関数
*/
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrecInstance, LPTSTR lpCmdLine, int nCmdShow)
{


	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//-- 画面サイズ設定 --
	if (WindowInfo::m_DebugGUIFlag)
	{
		//-- GUI ON --
		WindowInfo::m_GUIWindowWidth = 540;
		WindowInfo::m_GUIWindowHeight = 260;
	}
	else
	{
		//-- GUI OFF --
		WindowInfo::m_GUIWindowWidth = 0;
		WindowInfo::m_GUIWindowHeight = 0;
	}

	//-- 変数宣言 --
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//-- ウィンドウ作成 --
	if (FAILED(DX::DirectXManager::GetInstance().MakeWindow(hInstance, nCmdShow)))
	{
		DX::MakeMessage(L"ウィンドウ初期化失敗");
		return -1;
	}

	//-- フレームカウント初期化 --
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	//-- DirectX初期化 --
	if (FAILED(DX::DirectXManager::GetInstance().Init()))
	{
		DX::MakeMessage(L"DirectX初期化失敗 ");
		return -1;
	}

	//-- メッセージループ --
	MSG msg;
	while (node::Application::GetGameFlag())
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 1000)
			{	// 1秒ごとに実行
				DX::DirectXManager::GetInstance().m_CountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				//-- FPS更新 --
				DX::DirectXManager::GetInstance().SetFPS((float)dwFrameCount);
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;
				//-- 更新処理 --
				DX::DirectXManager::GetInstance().Update();
			}
			//-- 描画処理 --
			DX::DirectXManager::GetInstance().Draw();

			dwFrameCount++;
		}
	}

	//-- 終了処理 --
	DX::DirectXManager::GetInstance().Uninit();

	return (int)msg.wParam;
}

/*
	ウィンドウ作成時に呼ばれる関数
	画面サイズ設定
*/
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
	//-- 画面領域サイズ設定 --
	RECT rcClnt;
	GetClientRect(hWnd, &rcClnt);
	rcClnt.right -= rcClnt.left;
	rcClnt.bottom -= rcClnt.top;
	if (rcClnt.right != WindowInfo::m_ScreenWidth + WindowInfo::m_GUIWindowWidth
		|| rcClnt.bottom != WindowInfo::m_ScreenHeight + WindowInfo::m_GUIWindowHeight)
	{
		RECT rcWnd;
		GetWindowRect(hWnd, &rcWnd);
		SIZE sizeWnd;
		sizeWnd.cx = (rcWnd.right - rcWnd.left) - rcClnt.right + WindowInfo::m_ScreenWidth + WindowInfo::m_GUIWindowWidth;
		sizeWnd.cy = (rcWnd.bottom - rcWnd.top) - rcClnt.bottom + WindowInfo::m_ScreenHeight + WindowInfo::m_GUIWindowHeight;
		SetWindowPos(hWnd, nullptr, 0, 0, sizeWnd.cx, sizeWnd.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	return 0;
}

/*
	コールバック関数
*/
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg) {
	case WM_CREATE:
		return OnCreate(hWnd, (LPCREATESTRUCT)lParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		POINT point;
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		KeyInput::SetMousePos(point);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		}
		break;
	case WM_MENUCHAR:
		return MNC_CLOSE << 16;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

