//----------------------------------
//	クォータニオン
//-- author --
//	HatoriMasahi
//----------------------------------

//-- include --
#include "Quaternion.h"

//-- 静的メンバ --
const DirectX::XMVECTOR Quaternion::Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Down = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Left = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const DirectX::XMVECTOR Quaternion::Back = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

/*
	コンストラクタ
*/
Quaternion::Quaternion()
{
	Identity();
}

/*
	初期化
*/
void Quaternion::Identity()
{
	w = 1.0f;
	x = y = z = 0.0f;
}

/*
	正規化
*/
void Quaternion::Normalize()
{
	//-- 四元数の大きさ --
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
		DX::MakeMessage(L"クォータニオンの長さが0");
		Identity();
	}
}

/*
	クォータニオンセット from オイラー角
	引数	変換したいオイラークラス変数
*/
void Quaternion::SetQuaternion(const EulerAngles &angle)
{
	float sinPitch = sinf(angle.m_Pitch / 2);
	float cosPitch = cosf(angle.m_Pitch / 2);
	float sinHead = sinf(angle.m_Heading / 2);
	float cosHead = cosf(angle.m_Heading / 2);
	float sinBank = sinf(angle.m_Bank / 2);
	float cosBank = cosf(angle.m_Bank / 2);

	//-- 値の計算 --
	w = cosPitch * cosHead * cosBank + sinPitch * sinHead * sinBank;
	x = cosHead * sinPitch * cosBank + sinHead * cosPitch * sinBank;
	y = sinHead * cosPitch * cosBank - cosHead * sinPitch * sinBank;
	z = cosHead * cosPitch * sinBank - sinHead * sinPitch * cosBank;
}

/*
	クォータニオンセット from 任意軸 / 角度
	引数	回転の中心軸
			回転角度(ラジアン角)
*/
void Quaternion::SetToRotateAxisAngle(DirectX::XMVECTOR axis, float theta)
{
	//-- 回転軸の正規化 --
	axis = DirectX::XMVector3Normalize(axis);
	DirectX::XMFLOAT3 vec;
	DirectX::XMStoreFloat3(&vec, axis);

	float thetaOver2 = theta / 2.0f;
	float sinThetaOver2 = sinf(thetaOver2);

	//-- 値の設定 --
	w = cosf(thetaOver2);
	x = -vec.x * sinThetaOver2;
	y = -vec.y * sinThetaOver2;
	z = -vec.z * sinThetaOver2;
}

/*
	演算子オーバーロード
	クォータニオン同士の乗算
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
	演算子オーバーロード
	クォータニオン同士の代入乗算
*/
const Quaternion Quaternion::operator *= (const Quaternion& q)
{
	*this = *this * q;
	return *this;
}

/*
	倍率変化
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
	クォータニオン同士の内積
	引数	クォータニオンクラス変数 2つ
*/
float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

/*
	クォータニオン球面線形補間
	引数	クォータニオンクラス変数 2つ
			補間の比率	(0.0 ~ 1.0)
*/
Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float ratio)
{
	//-- 比率が範囲外の場合はそのまま返す --
	if (ratio <= 0.0f) return q1;
	if (ratio >= 1.0f) return q2;

	//-- 四元数同士の内積 --
	float cosOmega = Quaternion::Dot(q1, q2);

	//-- 角度の選択 --
	Quaternion qTemp = q1;
	Quaternion qTemp2 = q2;
	if (cosOmega < 0.0f)
	{
		qTemp *= -1;
		cosOmega *= -1;
	}

	//-- 補間用フラクション算出 --
	float k0, k1;
	if (cosOmega > 0.9999f)
	{
		//-- クォータニオン同士が近い場合 --
		k0 = 1.0f - ratio;	//単純な線形補間
		k1 = ratio;
	}
	else
	{
		//-- sin 算出 --
		float sinOmega = sqrtf(1.0f - powf(cosOmega, 2.0f));

		//-- 角度算出 --
		float omega = atan2f(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		//-- 補間パラメータ算出 --
		k0 = sinf((1.0f - ratio) * omega) * oneOverSinOmega;
		k1 = sinf(ratio * omega) * oneOverSinOmega;
	}

	//-- 補間 --
	Quaternion ret;
	ret.w = k0 * qTemp.w + k1 * qTemp2.w;
	ret.x = k0 * qTemp.x + k1 * qTemp2.x;
	ret.y = k0 * qTemp.y + k1 * qTemp2.y;
	ret.z = k0 * qTemp.z + k1 * qTemp2.z;

	return ret;
}

/*
	共役四元数
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
	回転マトリクスゲット
*/
DirectX::XMMATRIX Quaternion::GetRotateMatrix()
{
	DirectX::XMMATRIX retMat;
	retMat = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 TempMat;
	DirectX::XMStoreFloat4x4(&TempMat, retMat);

	//-- 演算部 --
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


