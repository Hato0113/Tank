//--------------------------
//	トランスフォームクラス
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Transform.h"

/*
	コンストラクタ
*/
Transform::Transform()
{
	m_Pos = { 0.0f,0.0f,0.0f };
	m_EulerAngle.Identity();
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Name = "Default";
	m_Tag = "Default";
}

/*
	名前指定コンストラクタ
*/
Transform::Transform(std::string name) : Transform()
{
	m_Name = name;
}

/*
	名前 / タグ指定コンストラクタ
*/
Transform::Transform(std::string name, std::string tag) : Transform(name)
{
	m_Tag = tag;
}

/*
	座標の加算
	引数 : 加算する値
*/
void Transform::AddPos(const DirectX::XMFLOAT3 add)
{
	m_Pos.x += add.x;
	m_Pos.y += add.y;
	m_Pos.z += add.z;
}