//----------------------------------
//	�N�H�[�^�j�I��
//-- author --
//	HatoriMasahi
//----------------------------------

//-- include --
#include "Quaternion.h"

//-- �ÓI�����o --
const DirectX::XMVECTOR Quaternion::Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Down = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Left = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Back = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

/*
	�R���X�g���N�^
*/
Quaternion::Quaternion()
{
	Identity();
}

/*
	������
*/
void Quaternion::Identity()
{
	w = 1.0f;
	x = y = z = 0.0f;
}

/*
	���K��
*/
void Quaternion::Normalize()
{
	//-- �l�����̑傫�� --
	float mag = (float)sqrt(w * w + x * x + y * y + z * z);

	if (mag > 0.0f)
	{
		float oneOverMag = 1.0f / mag;
		w *= oneOverMag;
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;
	}
	else
	{
		DX::MakeMessage(L"�N�H�[�^�j�I���̒�����0");
		Identity();
	}
}

/*
	�N�H�[�^�j�I���Z�b�g from �I�C���[�p
	����	�ϊ��������I�C���[�N���X�ϐ�
*/
void Quaternion::SetQuaternion(const EulerAngles &angle)
{
	float sinPitch = sinf(angle.m_Pitch / 2);
	float cosPitch = cosf(angle.m_Pitch / 2);
	float sinHead = sinf(angle.m_Heading / 2);
	float cosHead = cosf(angle.m_Heading / 2);
	float sinBank = sinf(angle.m_Bank / 2);
	float cosBank = cosf(angle.m_Bank / 2);

	//-- �l�̌v�Z --
	w = cosPitch * cosHead * cosBank + sinPitch * sinHead * sinBank;
	x = cosHead * sinPitch * cosBank + sinHead * cosPitch * sinBank;
	y = sinHead * cosPitch * cosBank - cosHead * sinPitch * sinBank;
	z = cosHead * cosPitch * sinBank - sinHead * sinPitch * cosBank;
}

/*
	�N�H�[�^�j�I���Z�b�g from �C�ӎ� / �p�x
	����	��]�̒��S��
			��]�p�x(���W�A���p)
*/
void Quaternion::SetToRotateAxisAngle(DirectX::XMVECTOR axis, float theta)
{
	//-- ��]���̐��K�� --
	axis = DirectX::XMVector3Normalize(axis);
	DirectX::XMFLOAT3 vec;
	DirectX::XMStoreFloat3(&vec, axis);

	float thetaOver2 = theta / 2.0f;
	float sinThetaOver2 = sinf(thetaOver2);

	//-- �l�̐ݒ� --
	w = cosf(thetaOver2);
	x = -vec.x * sinThetaOver2;
	y = -vec.y * sinThetaOver2;
	z = -vec.z * sinThetaOver2;
}

/*
	���Z�q�I�[�o�[���[�h
	�N�H�[�^�j�I�����m�̏�Z
*/
const Quaternion Quaternion::operator * (const Quaternion& q) const
{
	Quaternion ret;
	ret.w = w * q.w - x * q.x - y * q.y - z * q.z;
	ret.x = w * q.x + x * q.w + z * q.y - y * q.z;
	ret.y = w * q.y + y * q.w + x * q.z - z * q.x;
	ret.z = w * q.z + z * q.w + y * q.x - x * q.y;
	return ret;
}

/*
	���Z�q�I�[�o�[���[�h
	�N�H�[�^�j�I�����m�̑����Z
*/
const Quaternion Quaternion::operator *= (const Quaternion& q)
{
	*this = *this * q;
	return *this;
}

/*
	�{���ω�
*/
const Quaternion Quaternion::operator*= (float mag)
{
	this->w *= mag;
	this->x *= mag;
	this->y *= mag;
	this->z *= mag;
	return *this;
}

/*
	�N�H�[�^�j�I�����m�̓���
	����	�N�H�[�^�j�I���N���X�ϐ� 2��
*/
float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

/*
	�N�H�[�^�j�I�����ʐ��`���
	����	�N�H�[�^�j�I���N���X�ϐ� 2��
			��Ԃ̔䗦	(0.0 ~ 1.0)
*/
Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float ratio)
{
	//-- �䗦���͈͊O�̏ꍇ�͂��̂܂ܕԂ� --
	if (ratio <= 0.0f) return q1;
	if (ratio >= 1.0f) return q2;

	//-- �l�������m�̓��� --
	float cosOmega = Quaternion::Dot(q1, q2);

	//-- �p�x�̑I�� --
	Quaternion qTemp = q1;
	Quaternion qTemp2 = q2;
	if (cosOmega < 0.0f)
	{
		qTemp *= -1;
		cosOmega *= -1;
	}

	//-- ��ԗp�t���N�V�����Z�o --
	float k0, k1;
	if (cosOmega > 0.9999f)
	{
		//-- �N�H�[�^�j�I�����m���߂��ꍇ --
		k0 = 1.0f - ratio;	//�P���Ȑ��`���
		k1 = ratio;
	}
	else
	{
		//-- sin �Z�o --
		float sinOmega = sqrtf(1.0f - powf(cosOmega, 2.0f));

		//-- �p�x�Z�o --
		float omega = atan2f(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		//-- ��ԃp�����[�^�Z�o --
		k0 = sinf((1.0f - ratio) * omega) * oneOverSinOmega;
		k1 = sinf(ratio * omega) * oneOverSinOmega;
	}

	//-- ��� --
	Quaternion ret;
	ret.w = k0 * qTemp.w + k1 * qTemp2.w;
	ret.x = k0 * qTemp.x + k1 * qTemp2.x;
	ret.y = k0 * qTemp.y + k1 * qTemp2.y;
	ret.z = k0 * qTemp.z + k1 * qTemp2.z;

	return ret;
}

/*
	�����l����
*/
Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	Quaternion ret;
	ret.w = q.w;
	ret.x = q.x;
	ret.y = q.y;
	ret.z = q.z;
	return ret;
}

/*
	��]�}�g���N�X�Q�b�g
*/
DirectX::XMMATRIX Quaternion::GetRotateMatrix()
{
	DirectX::XMMATRIX retMat;
	retMat = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 TempMat;
	DirectX::XMStoreFloat4x4(&TempMat, retMat);

	//-- ���Z�� --
	TempMat._11 = 1.0f - 2.0f * (y * y + z * z);
	TempMat._12 = 2.0f * (x * y - w * z);
	TempMat._13 = 2.0f * (x * z + w * y);
	
	TempMat._21 = 2.0f * (x * y + w * z);
	TempMat._22 = 1.0f - 2.0f * (x * x + z * z);
	TempMat._23 = 2.0f * (y * z - w * x);

	TempMat._31 = 2.0f * (x * z - w * y);
	TempMat._32 = 2.0f * (y * z + w * x);
	TempMat._33 = 1.0f - 2.0f * (x * x + y * y);

	retMat = DirectX::XMLoadFloat4x4(&TempMat);
	return retMat;
}


