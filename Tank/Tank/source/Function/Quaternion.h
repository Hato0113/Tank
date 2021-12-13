//----------------------------------
//	�N�H�[�^�j�I��
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once

/*
	�N�H�[�^�j�I���N���X
*/

//-- include --
#include "MySystem\DirectX.h"
#include "EulerAngles.h"

//-- �N���X��` --
class EulerAngles;
class Quaternion final
{
public:
	Quaternion();
	virtual ~Quaternion() = default;

	//-- �����o�ϐ� --
public:
	union
	{
		struct
		{
			float w;
			float x;
			float y;
			float z;
		};
		float Array[4];
	};

	//-- �ÓI�����o --
	const static DirectX::XMVECTOR Down;
	const static DirectX::XMVECTOR Up;
	const static DirectX::XMVECTOR Right;
	const static DirectX::XMVECTOR Left;
	const static DirectX::XMVECTOR Forward;
	const static DirectX::XMVECTOR Back;

	//-- ���\�b�h --
public:
	void Identity();	//������
	void Normalize();	//���K��
	void SetQuaternion(const EulerAngles&);		//�I�C���[�p����̃Z�b�g
	void SetToRotateAxisAngle(DirectX::XMVECTOR,float theta);	//���𒆐S�Ƃ�����]�N�H�[�^�j�I���Z�b�g
	DirectX::XMMATRIX GetRotateMatrix();		//���g�̉�]�}�g���N�X��^����

	const Quaternion operator * (const Quaternion&) const;
	const Quaternion operator *= (const Quaternion&);
	const Quaternion operator *= (float mag);	//�{���ω�

	//-- �ÓI���\�b�h --
public:
	static float Dot(const Quaternion& q1, const Quaternion& q2);		//����
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float ratio);	//���ʐ��`���
	static Quaternion Conjugate(const Quaternion& q);	//�����l����
};