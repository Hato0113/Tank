//----------------------------------
//	IMGUI - ヒエラルキービュー
//-- author --
//	HatoriMasahi
//----------------------------------

//-- include --
#include "GUI_Hierarchy.h"
using namespace IG;
#include "MySystem\main.h"
#include "Scene\SceneManager.h"
#include <algorithm>

Hierarchy* Hierarchy::m_Hierarchy = nullptr;

Hierarchy* Hierarchy::Create()
{
	if (m_Hierarchy) return nullptr;

	m_Hierarchy = new Hierarchy();
	m_Hierarchy->Init();

	return m_Hierarchy;
}

void Hierarchy::Destroy()
{
	if (!m_Hierarchy) return;

	m_Hierarchy->Uninit();
	delete m_Hierarchy;
	m_Hierarchy = nullptr;
}

void Hierarchy::Init()
{
	m_Active = false;
	m_CurrentSelect = 0;
	m_PrevSelect = -1;
}

void Hierarchy::Update()
{

}

void Hierarchy::Draw()
{
	if (!m_Active) return;

	//-- ウィンドウ設定 --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(100.0f,100.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f), ImGuiCond_Once);

	ImGui::Begin("Hierarchy", &m_Active);

	//-- 描画用オブジェクトリスト生成 --
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	std::vector<std::string> ObjectNameList;
	if (scene)
	{
		auto objList = scene->manager->GetList();
		if(!objList.empty())
		for (auto it = objList.begin();it != objList.end();it++)
		{
			auto count = std::count_if(objList.begin(), it, [&it](Object* obj) {
				return obj->transform->GetName() == (*it)->transform->GetName();
				});
			std::string name = (*it)->transform->GetName();
			if (count > 0)
			{
				char buf[8];
				name += _itoa(static_cast<int>(count), buf, 10);	//IMGUIのラベル管理のために番号をつける
			}
			ObjectNameList.push_back(name);
		}
	}

	//-- 情報描画 --
	if (scene)
	{
		//-- リスト表示 --
		if(ImGui::BeginListBox("ObjectList"))
		{
			if (m_CurrentSelect > ObjectNameList.size())
				m_CurrentSelect = 0;
			for (int i = 0; i < ObjectNameList.size(); i++)
			{
				const bool is_selected = (m_CurrentSelect == i);
				if (ImGui::Selectable(ObjectNameList[i].c_str(), is_selected))
					m_CurrentSelect = i;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		auto objList = scene->manager->GetList();
		static bool active;
		static bool visible;
		if (m_CurrentSelect != m_PrevSelect)
		{
			active = objList[m_CurrentSelect]->GetActive();
			visible = objList[m_CurrentSelect]->GetVisible();
			m_PrevSelect = m_CurrentSelect;
		}
		//-- アイテム詳細表示 --
		if (ImGui::TreeNode("Detail"))
		{
			ImGui::Text("Name : %s", objList[m_CurrentSelect]->transform->GetName());
			ImGui::Text("Tag : %s", objList[m_CurrentSelect]->transform->GetTag());
			auto pos = objList[m_CurrentSelect]->transform->GetPos();
			ImGui::Text("Pos : %.3f / %.3f / %.3f", pos.x, pos.y, pos.z);
			ImGui::Checkbox("Active", &active);
			objList[m_CurrentSelect]->SetActive(active);
			ImGui::Checkbox("Visible", &visible);
			objList[m_CurrentSelect]->SetVisible(visible);
		}
	}


	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Hierarchy::Uninit()
{

}
