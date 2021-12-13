//----------------------------------
//	頂点シェーダーベース
//-- author --
//	HatoriMasashi
//----------------------------------

//-- include --
#include "VertexBase.h"
#include "MySystem\DirectX.h"

VertexBase::VertexBase()
{
	m_Shader = nullptr;
	m_Layout = nullptr;
}

VertexBase::~VertexBase()
{
	if (m_Shader)
	{
		m_Shader->Release();
		m_Shader = nullptr;
	}
	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = nullptr;
	}
}

/*
	自身のバインド
*/
void VertexBase::Bind()
{
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	pDC->VSSetShader(m_Shader, nullptr, 0);
	pDC->IASetInputLayout(m_Layout);
}


