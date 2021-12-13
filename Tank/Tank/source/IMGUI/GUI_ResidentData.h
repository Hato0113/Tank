//----------------------------
//	常駐データ管理
//-- author --
//	HatoriMasashi
//----------------------------
#pragma once

//-- include --
#include "MyImGui.h"
#include "GUI_EditorLauncher.h"

namespace IG
{
	class ResidentDataGUI;
}

/*
	常駐データをGUIに反映
*/
class IG::ResidentDataGUI : public IG::Editor
{
private:
	ResidentDataGUI() {}
public:
	static ResidentDataGUI* Create();
	void Destroy() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	static ResidentDataGUI* m_ResidentDataManager;	//オブジェクトはこれのみ
};
