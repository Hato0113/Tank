//----------------------------------
//	IMGUI - ヒエラルキービュー
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once
//-- include --
#include "MyImGui.h"
#include "GUI_EditorLauncher.h"

namespace IG
{
	class Hierarchy;
}

//-- クラス定義 --
class IG::Hierarchy : public IG::Editor
{
private:
	Hierarchy() {}
public:
	static Hierarchy* Create();
	void Destroy() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	//-- 変数 --
	int m_CurrentSelect;
	int m_PrevSelect;

	static Hierarchy* m_Hierarchy;	//オブジェクトはこれのみ
};


