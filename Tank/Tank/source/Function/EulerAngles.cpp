//----------------------------------
//	�I�C���[�p�N���X
//-- author --
//	HatoriMasahi
//----------------------------------

//-- include --
#include "EulerAngles.h"
using namespace DirectX;

/*
	�R���X�g���N�^
*/
EulerAngles::EulerAngles(float y,float x,float z)
{
	m_Heading = y;
	m_Pitch = x;
	m_Bank = z;
}

/*
	������
*/
void EulerAngles::Identity()
{
	m_Heading = m_Pitch = m_Bank = 0.0f;
}


/*
	������
*/
void EulerAngles::Canonize()
{
	//-- �s�b�`�̃��b�v�� --
	m_Pitch = DX::wrapPI(m_Pitch);

	//-- pitch�̗������`�F�b�N --
	if (m_Pitch < -1.0f / XM_PI)
	{
		m_Pitch = -XM_PI - m_Pitch;
		m_Heading += XM_PI;
		m_Bank += XM_PI;
	}
	else if (m_Pitch > 1.0f / XM_PI)
	{
		m_Pitch = XM_PI - m_Pitch;
		m_Heading += XM_PI;
		m_Bank += XM_PI;
	}

	//-- �W���o�����b�N�`�F�b�N --
	if (fabsf(m_Pitch) > (1.0f / XM_PI) - 0.00001f)
	{
		m_Heading += m_Bank;
		m_Bank = 0.0f;
	}
	else
	{
		m_Bank = DX::wrapPI(m_Bank);
	}

	//-- �w�f�B���O�̐����͈̓��b�v�� --
	m_Heading = DX::wrapPI(m_Heading);
}

/*
	�I�C���[�p�̐ݒ� from Quaternion
*/
void EulerAngles::SetEulerAngles(const Quaternion& q)
{
	//-- sin(Pitch)�̎��o�� --
	float sinPitch = -2.0f * (q.y * q.z - q.w * q.x);

	//-- �W���o�����b�N�`�F�b�N --
	if (fabsf(sinPitch) > 0.9999f)
	{
		m_Pitch = XM_PI / 2.0f * sinPitch;
		float a = -q.x * q.z + q.w * q.y;
		float b = 0.5f - q.y * q.y - q.z * q.z;
		m_Heading = atan2f(a, b);
		m_Bank = 0.0f;
	}
	else
	{
		//-- �p�x�v�Z --
		m_Pitch = asinf(sinPitch);
		m_Heading = atan2f(q.x * q.z + q.w * q.y, 0.5f - q.x * q.x - q.y * q.y);
		m_Bank = atan2f(q.x * q.y + q.w * q.z, 0.5f - q.x * q.x - q.z * q.z);
	}
}

/*
	��]�s��m��
	�߂�l	��]�s��
*/
DirectX::XMMATRIX EulerAngles::GetRotateMatrix()
{
	Quaternion q;	//��������N�H�[�^�j�I���ɒ����āA�n���B
	q.SetQuaternion(*this);
	return q.GetRotateMatrix();
}

/*
	�����x�N�g���ԋp�֐�
*/
DirectX::XMVECTOR EulerAngles::GetDirVectorX()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, GetRotateMatrix());
	DirectX::XMFLOAT3 ret = { mat._11,mat._12,mat._13 };
	return DirectX::XMLoadFloat3(&ret);
}
DirectX::XMVECTOR EulerAngles::GetDirVectorY()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, GetRotateMatrix());
	DirectX::XMFLOAT3 ret = { mat._21,mat._22,mat._23 };
	return DirectX::XMLoadFloat3(&ret);
}
DirectX::XMVECTOR EulerAngles::GetDirVectorZ()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, GetRotateMatrix());
	DirectX::XMFLOAT3 ret = { mat._31,mat._32,mat._33 };
	return DirectX::XMLoadFloat3(&ret);
}
