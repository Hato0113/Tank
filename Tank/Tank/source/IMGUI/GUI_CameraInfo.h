//----------------------------------
//	GUI_CameraInfo.h
//	カメラ情報クラス
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once
//-- include --
#include "MyImGui.h"
#include <list>
#include "GUI_EditorLauncher.h"

namespace IG
{
	class CameraInfo;
}

//-- クラス定義 --
class IG::CameraInfo : public IG::Editor
{
private:
	CameraInfo() {}
public:
	static CameraInfo* Create();
	void Destroy() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	static CameraInfo* m_CameraInfo;	//オブジェクトはこれのみ
};


