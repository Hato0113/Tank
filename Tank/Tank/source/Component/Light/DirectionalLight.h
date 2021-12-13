//--------------------------
//	平行光源
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "MySystem\DirectX.h"
#include "Component\Component.h"

//-- クラス定義 --
class DirectionalLight : public Component
{
public:
	DirectionalLight();
	virtual ~DirectionalLight() = default;

	DirectX::XMFLOAT3 GetDir();
	DirectX::XMFLOAT4 GetDiffuse() { return m_Diffuse; }
	DirectX::XMFLOAT4 GetAmbient() { return m_Ambient; }
	DirectX::XMFLOAT4 GetSpecular() { return m_Specular; }
	void SetEnable(bool set) { m_Enable = set; }

	//-- メンバ変数 --
public:
	DirectX::XMFLOAT3 m_Direction;
	DirectX::XMFLOAT4 m_Ambient;
	DirectX::XMFLOAT4 m_Specular;
	DirectX::XMFLOAT4 m_Diffuse;
	bool m_Enable;	//true有効 / false無効
};
