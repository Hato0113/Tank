//--------------------------
//	平行光源
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	const DirectX::XMFLOAT3 DefaultDir{ -0.7f,-1.0f,0.8f };
	m_Direction = DefaultDir;
	m_Enable = true;

	//-- ライト色初期化 --
	const DirectX::XMFLOAT4 DefaultDiffuse(1, 1, 1, 1);
	const DirectX::XMFLOAT4 DefaultAmbient(0, 0, 0, 1);
	const DirectX::XMFLOAT4 DefaultSpecular(1, 1, 1, 1);
	m_Diffuse = DefaultDiffuse;
	m_Ambient = DefaultAmbient;
	m_Specular = DefaultSpecular;
}

DirectX::XMFLOAT3 DirectionalLight::GetDir()
{
	if (m_Enable)
		return m_Direction;
	else
		return DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
}

