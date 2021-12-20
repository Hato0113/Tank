//----------------------------
//	レベルエディター用GUI
//-- author --
//	HatoriMasashi
//----------------------------
#pragma once

//-- include --
#include "MyImGui.h"
#include "GUI_EditorLauncher.h"

namespace IG
{
	class LevelEditorGUI;
}

/*
	常駐データをGUIに反映
*/
class IG::LevelEditorGUI : public IG::Editor
{
private:
	LevelEditorGUI() {}
public:
	static LevelEditorGUI* Create();
	void Destroy() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	static LevelEditorGUI* m_LevelEditor;	//オブジェクトはこれのみ
	std::string m_CurrentSelect;
	std::string m_PrevSelect;
};
