//----------------------------------
//	オイラー角クラス
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
	Transformコンポーネントに角度情報を持たせるため、
	慣性空間の回転情報としてあつかう。
*/

//-- クラス定義 --
class Quaternion;
class EulerAngles final
{
public:
	EulerAngles() { Identity(); }
	EulerAngles(float y,float x,float z);
	virtual ~EulerAngles() = default;

	//-- メンバ変数 --
public:
	float m_Pitch;		//x軸中心のラジアン角   ピッチ
	float m_Heading;	//y軸中心のラジアン角	ヨー
	float m_Bank;		//z軸中心のラジアン角   ロール

	//-- メソッド --
public:
	void Identity();	//初期化
	void Canonize();	//正準値への設定
	void SetEulerAngles(const Quaternion& q);	//クォータニオンからのセット
	DirectX::XMMATRIX GetRotateMatrix();	//回転行列獲得
	DirectX::XMVECTOR GetDirVectorX();		//x+方向へのベクトル
	DirectX::XMVECTOR GetDirVectorY();		//y+方向へのベクトル
	DirectX::XMVECTOR GetDirVectorZ();		//z+方向へのベクトル
};