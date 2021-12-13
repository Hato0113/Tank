//----------------------------------
//	GUI_CameraInfo.cpp
//	カメラ情報クラス
//-- author --
//	HatoriMasahi
//----------------------------------

//-- include --
#include "GUI_CameraInfo.h"
#include "MySystem\main.h"
using namespace IG;
#include "Component\Camera\Camera.h"
#include "Scene\SceneManager.h"

CameraInfo* CameraInfo::m_CameraInfo = nullptr;


CameraInfo* CameraInfo::Create()
{
	if (m_CameraInfo != nullptr) return nullptr;

	m_CameraInfo = new CameraInfo();
	m_CameraInfo->Init();

	return m_CameraInfo;
}

void CameraInfo::Destroy()
{
	if (m_CameraInfo == nullptr) return;

	m_CameraInfo->Uninit();
	delete m_CameraInfo;
	m_CameraInfo = nullptr;
}

void CameraInfo::Init()
{

}

void CameraInfo::Update()
{

}

void CameraInfo::Draw()
{
	if (!m_Active) return;
	//-- ウィンドウ設定 --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2((float)WindowInfo::m_ScreenWidth, (float)200.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)WindowInfo::m_GUIWindowWidth, (float)300.0f), ImGuiCond_Once);

	ImGui::Begin("CameraInfo", &m_Active);

	//-- 情報描画 --
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	if (scene)
	{
		auto manager = scene->manager;
		if (manager)
		{
			auto obj = manager->FindObjectWithName("MainCamera");
			if (obj)
			{
				auto cam = obj->GetComponent<Camera>();
				auto target = cam->GetTarget();
				auto dirVec = cam->GetDir();
				auto pos = cam->parent->transform->GetPos();
				ImGui::Text("pos,xyz : %.3f / %.3f / %.3f", pos.x, pos.y, pos.z);
				ImGui::Text("target,xyz : %.3f / %.3f / %.3f", target.x, target.y, target.z);
				ImGui::Text("dirVec,xyz : %.3f / %.3f / %.3f", dirVec.x, dirVec.y, dirVec.z);
			}
		}
	}


	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void CameraInfo::Uninit()
{

}

