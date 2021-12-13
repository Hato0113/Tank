//----------------------------------
//	�I�C���[�p�N���X
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once

/*

*/

//-- include --
#include "MySystem\DirectX.h"
#include "Quaternion.h"

/*
	Transform�R���|�[�l���g�Ɋp�x�����������邽�߁A
	������Ԃ̉�]���Ƃ��Ă������B
*/

//-- �N���X��` --
class Quaternion;
class EulerAngles final
{
public:
	EulerAngles() { Identity(); }
	EulerAngles(float y,float x,float z);
	virtual ~EulerAngles() = default;

	//-- �����o�ϐ� --
public:
	float m_Pitch;		//x�����S�̃��W�A���p   �s�b�`
	float m_Heading;	//y�����S�̃��W�A���p	���[
	float m_Bank;		//z�����S�̃��W�A���p   ���[��

	//-- ���\�b�h --
public:
	void Identity();	//������
	void Canonize();	//�����l�ւ̐ݒ�
	void SetEulerAngles(const Quaternion& q);	//�N�H�[�^�j�I������̃Z�b�g
	DirectX::XMMATRIX GetRotateMatrix();	//��]�s��l��
	DirectX::XMVECTOR GetDirVectorX();		//x+�����ւ̃x�N�g��
	DirectX::XMVECTOR GetDirVectorY();		//y+�����ւ̃x�N�g��
	DirectX::XMVECTOR GetDirVectorZ();		//z+�����ւ̃x�N�g��
};