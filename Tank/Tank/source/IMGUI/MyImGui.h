//----------------------------------
// MyImGui.h
//-- author --
//	HatoriMasahi
//----------------------------------

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <list>

namespace IG
{
	class IMGUIManager;
}

class IG::IMGUIManager
{
private:
	IMGUIManager();
	~IMGUIManager() = default;
public:
	IMGUIManager(const IMGUIManager&) = delete;
	IMGUIManager& operator=(const IMGUIManager&) = delete;
	IMGUIManager(IMGUIManager&&) = delete;
	IMGUIManager& operator=(IMGUIManager&&) = delete;

	static IMGUIManager& GetInstance()
	{
		static IMGUIManager Inst;
		return Inst;
	}

	const HRESULT Init();
	const void Uninit();
	const void Draw();
	const void Update();
};

