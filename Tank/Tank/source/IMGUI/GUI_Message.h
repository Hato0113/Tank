//----------------------------------
//	GUI_Message.h
//	���b�Z�[�W�E�B���h�E�N���X
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once
//-- include --
#include "MyImGui.h"
#include <list>
#include "GUI_EditorLauncher.h"
#include <vector>

namespace IG
{
	class Message;
	class MessageManager;
}

/*
	GUI���b�Z�[�W�N���X
	�e�I�u�W�F�N�g���瑗��ꂽ���b�Z�[�W�����X�g�Ǘ�
	���b�Z�[�W��ID�Ǘ����A�X�V�֐����Ă΂�Ă���Ԃ͓��ꃁ�b�Z�[�W�Ƃ��Ĉ���
	Unity��Debug.Log���Q�l�ɁB
*/

//-- �N���X��` --
/*
	���b�Z�[�W�I�u�W�F�N�g�N���X
	���������b�Z�[�W�Ƃ��ĕۑ�
*/
class IG::Message
{
	friend class IG::MessageManager;
private:
	char m_szMessage[256];
	int m_LifeTime;
	int m_MessageLen;
	char m_szOriginalMessage[256];
};

/*
	���b�Z�[�W�Ǘ��N���X
	���g�̃|�C���^��ÓI�����o�Ƃ��Ď����Ƃ�
	�^���V���O���g���I�u�W�F�N�g�Ƃ��Đ���
*/
class IG::MessageManager : public Editor
{
private:
	MessageManager() {}
public:
	static void DrawLog(const char* message, ...);	//���t���[���ŏ�����A�f�[�^���܂ރ��b�Z�[�W
	static void DrawSystemLog(std::string message);	//�����Ȃ��P�ꃁ�b�Z�[�W
	static MessageManager* Create();
	void Destroy() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	std::list<Message*> m_MessageList;
	std::vector<std::string> m_SystemLog;
	static MessageManager* m_MessageManager;	//�I�u�W�F�N�g�͂���̂�
	static bool CreateSameMessageFlag;
};


