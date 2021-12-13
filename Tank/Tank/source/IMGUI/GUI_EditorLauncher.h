//----------------------------------
//	GUI_EditorLauncher.h
//	�G�f�B�^�[�����`���[
//	IMGUI�̃E�B���h�E���ꊇ�Ǘ�
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once
//-- include --
#include "MyImGui.h"
#include <map>

namespace IG
{
	class Launcher;
	class Editor;
}

/*
	IMGUI�����Ǘ��N���X
	�E�B���h�E�̍쐬 / �A�N�e�B�u��Ԃ̐؂�ւ������s��
*/

//-- �N���X��` --
/*
	IMGUI�E�B���h�E�Ǘ��N���X
	�ÓI���\�b�h�݂̂�����
*/
class IG::Editor;
class IG::Launcher final
{
	Launcher() = delete;
public:
	static void Init();
	static void Update();
	static void Draw();
	static void Uninit();

	static std::map<std::string, IG::Editor*> m_WindowMap;
	static bool m_WindowFlag;
};

/*
	�G�f�B�^�[�N���X
	���̃N���X�Ɗ��Ƃ����E�B���h�E��Launcher�N���X�ŊǗ�����B
	��x�o�^���ꂽ���̂�[x]�������Ă���\���ɂȂ���̂Ƃ���B
*/
class IG::Editor
{
public:
	Editor() {}
	virtual ~Editor() {}
public:
	virtual void Destroy() = 0;
	virtual void Init() = 0;	//������
	virtual void Uninit() = 0;	//�I������
	virtual void Update() = 0;	//�X�V����
	virtual void Draw() = 0;	//�`�揈��
	bool m_Active = true;
};


