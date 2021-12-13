//----------------------------------
//	GUI_Message.cpp
//	���b�Z�[�W�E�B���h�E�N���X
//-- author --
//	HatoriMasahi
//-- Log --
//	2021/06/14 �쐬
//----------------------------------

//-- include --
#define _CRT_SECURE_NO_WARNINGS
#include "GUI_Message.h"
#include "MySystem\main.h"
using namespace IG;

//-- �ÓI�����o --
MessageManager* MessageManager::m_MessageManager = nullptr;
bool MessageManager::CreateSameMessageFlag = false;

/*
	���O�Z�b�g�֐�
	�\�����郁�b�Z�[�W�̐ݒ���s��
*/
void MessageManager::DrawLog(const char* message, ...)
{
	if (m_MessageManager == nullptr) return;

	//-- ���b�Z�[�W��� --
	va_list list;
	va_start(list, message);
	char aBuf[256];
	_vsprintf_p(aBuf, sizeof(aBuf), message, list);
	va_end(list);
	int len = (int)strlen(message);

	for (auto temp : m_MessageManager->m_MessageList)
	{
		if (temp->m_MessageLen == len)
		{
			//-- �S���������b�Z�[�W������� --
			if (strcmp(message, temp->m_szOriginalMessage) == 0 && !CreateSameMessageFlag)
			{
				//-- �����ꍇ�̓��b�Z�[�W�̍쐬�͂��Ȃ� --
				temp->m_LifeTime = 180;		//180F�̊Ԃ͐���
				//-- ���b�Z�[�W�̍X�V --
				strcpy(temp->m_szMessage, aBuf);
				return;
			}
		}
	}

	//-- �V�������b�Z�[�W�Ƃ��ēo�^ --
	Message* msg = new Message();
	msg->m_MessageLen = len;
	strcpy(msg->m_szMessage, aBuf);
	msg->m_LifeTime = 180;	//�������Ԑݒ�
	strcpy(msg->m_szOriginalMessage, message);
	m_MessageManager->m_MessageList.push_back(msg);	//���X�g�o�^
}

/*
	�V�X�e�����O�ǉ�
	���� : ���O���b�Z�[�W
*/
void MessageManager::DrawSystemLog(std::string str)
{
	if (m_MessageManager == nullptr) return;
	m_MessageManager->m_SystemLog.push_back(str);
}


/*
	�E�B���h�E�쐬�֐�
*/
MessageManager* MessageManager::Create()
{
	if (m_MessageManager != nullptr) return nullptr;

	m_MessageManager = new MessageManager();
	m_MessageManager->Init();

	return m_MessageManager;
}

/*
	�E�B���h�E�폜
*/
void MessageManager::Destroy()
{
	if (m_MessageManager == nullptr) return;

	m_MessageManager->Uninit();
	delete m_MessageManager;
	m_MessageManager = nullptr;
}

/*
	�������֐�
*/
void MessageManager::Init()
{
	//-- �����o�ϐ������� --
	m_MessageList.clear();
}

/*
	�X�V�֐�
*/
void MessageManager::Update()
{
	for (auto itr = m_MessageList.begin(); itr != m_MessageList.end(); )
	{
		if ((*itr)->m_LifeTime-- == 0)	//�������Ԑ؂�
			itr = m_MessageList.erase(itr);
		else
			itr++;
	}
}

/*
	�`��֐�
*/
void MessageManager::Draw()
{
	if (!m_Active) return;
	//-- �E�B���h�E�ݒ� --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.7f / 2, 0.2f / 2, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(0.0f, (float)WindowInfo::m_ScreenHeight),ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)WindowInfo::m_ScreenWidth / 2, (float)WindowInfo::m_GUIWindowHeight), ImGuiCond_Once);

	ImGui::Begin("DebugMessage", &m_Active);

	for (auto it : m_MessageList)
	{
		ImGui::Text(it->m_szMessage);
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//-- �P�ꃁ�b�Z�[�W�`��p --
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.078f, 0.576f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(1.0f / 2, 0.078f / 2, 0.576f / 2, 1.0f));
	ImGui::SetNextWindowPos(ImVec2((float)WindowInfo::m_ScreenWidth / 2, (float)WindowInfo::m_ScreenHeight), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)WindowInfo::m_ScreenWidth / 2, (float)WindowInfo::m_GUIWindowHeight), ImGuiCond_Once);

	ImGui::Begin("SystemLog", &m_Active);

	if (ImGui::Button("LogDelete", { 100.0f,30.0f }))
	{
		m_SystemLog.clear();
	}
	ImGui::Separator();

	for (auto log : m_SystemLog)
	{
		ImGui::Text(log.c_str());
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

/*
	�I������
*/
void MessageManager::Uninit()
{
	//-- ���b�Z�[�W�I�u�W�F�N�g�폜 --
	auto tempList = m_MessageList;
	for (auto it : tempList)
	{
		delete it;
	}

	//-- ���X�g������ --
	m_MessageList.clear();
}
