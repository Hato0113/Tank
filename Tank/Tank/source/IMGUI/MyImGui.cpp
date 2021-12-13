//----------------------------------
// MyImGui.cpp
//-- author --
//	HatoriMasahi
//-- Log --
//	2021/04/06 �쐬
//----------------------------------

#define _CRT_SECURE_NO_WARNINGS
#include "MyImGui.h"
#include "MySystem\DirectX.h"
#include <vector>
#include "GUI_Message.h"
#include "GUI_EditorLauncher.h"
using namespace IG;

/*
	�R���X�g���N�^
*/
IMGUIManager::IMGUIManager()
{
	//none
}

/*
	IMGUI������
*/
const HRESULT IMGUIManager::Init()
{
	//-- IMGUI������ --
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	if (!ImGui_ImplWin32_Init(DX::DirectXManager::GetInstance().GetWndHandle()))
	{
		DX::MakeMessage(L"IMGUI/WIN32���������s");
		ImGui::DestroyContext();
		return S_FALSE;
	}

	if (!ImGui_ImplDX11_Init(DX::DirectXManager::GetInstance().GetDevice(), DX::DirectXManager::GetInstance().GetDeviceContext()))
	{
		DX::MakeMessage(L"IMGUI/DX11���������s");
		ImGui::DestroyContext();
		return S_FALSE;
	}

	io.IniFilename = NULL;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	//-- �eIMGUI�N���X�������֐� --
	Launcher::Init();

	return S_OK;
}

/*
	IMGUI�I������
*/
const void IMGUIManager::Uninit()
{
	Launcher::Uninit();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

/*
	IMGUI�`��
*/
const void IMGUIManager::Draw()
{
	if (!WindowInfo::m_DebugGUIFlag) return;

	//-- �`�揀�� --
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//-- �eIMGUI�N���X�`��֐� --
	RECT rc;		//testMessage
	GetWindowRect(DX::DirectXManager::GetInstance().GetWndHandle(), &rc);
	MessageManager::DrawLog("WindowRect : top = %d / bottom = %d / right = %d / left = %d", rc.top, rc.bottom, rc.right, rc.left);
	RECT size;
	GetClientRect(DX::DirectXManager::GetInstance().GetWndHandle(), &size);
	MessageManager::DrawLog("ClientRect : top = %d / bottom = %d / right = %d / left = %d", size.top, size.bottom, size.right, size.left);

	Launcher::Draw();

	//-- �`�� --
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/*
	IMGUI�X�V
*/
const void IMGUIManager::Update()
{
	Launcher::Update();
}