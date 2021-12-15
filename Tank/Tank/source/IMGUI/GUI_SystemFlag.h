//----------------------------
//	常駐システムフラグ管理
//-- author --
//	HatoriMasashi
//----------------------------
#pragma once

//-- include --
#include "MyImGui.h"
#include "GUI_EditorLauncher.h"

namespace IG
{
	class ResidentFlagGUI;
}

/*
	常駐フラグをGUIに反映
*/
class IG::ResidentFlagGUI : public IG::Editor
{
private:
	ResidentFlagGUI() {}
public:
	static ResidentFlagGUI* Create();
	void Destroy() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	static ResidentFlagGUI* m_ResidentFlagManager;	//オブジェクトはこれのみ
};

