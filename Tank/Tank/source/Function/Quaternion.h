//----------------------------------
//	クォータニオン
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once

/*
	クォータニオンクラス
*/

//-- include --
#include "MySystem\DirectX.h"
#include "EulerAngles.h"

//-- クラス定義 --
class EulerAngles;
class Quaternion final
{
public:
	Quaternion();
	virtual ~Quaternion() = default;

	//-- メンバ変数 --
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

	//-- 静的メンバ --
	const static DirectX::XMVECTOR Down;
	const static DirectX::XMVECTOR Up;
	const static DirectX::XMVECTOR Right;
	const static DirectX::XMVECTOR Left;
	const static DirectX::XMVECTOR Forward;
	const static DirectX::XMVECTOR Back;

	//-- メソッド --
public:
	void Identity();	//初期化
	void Normalize();	//正規化
	void SetQuaternion(const EulerAngles&);		//オイラー角からのセット
	void SetToRotateAxisAngle(DirectX::XMVECTOR,float theta);	//軸を中心とした回転クォータニオンセット
	DirectX::XMMATRIX GetRotateMatrix();		//自身の回転マトリクスを与える

	const Quaternion operator * (const Quaternion&) const;
	const Quaternion operator *= (const Quaternion&);
	const Quaternion operator *= (float mag);	//倍率変化

	//-- 静的メソッド --
public:
	static float Dot(const Quaternion& q1, const Quaternion& q2);		//内積
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float ratio);	//球面線形補間
	static Quaternion Conjugate(const Quaternion& q);	//共役四元数
};