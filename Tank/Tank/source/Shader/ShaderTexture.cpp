//----------------------------------
// シェーダー用テクスチャ
//-- author --
//	HatoriMasashi
//----------------------------------

//-- include --
#include "ShaderTexture.h"
#include "MySystem\DirectX.h"

ShaderTexture::ShaderTexture()
{
	m_Tex = nullptr;
	m_Slot = 0;
}

void ShaderTexture::Bind()
{
	auto pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	pDC->PSSetShaderResources(m_Slot, 1, &m_Tex);
}

void ShaderTexture::Set(ID3D11ShaderResourceView* tex, int slot)
{
	m_Tex = tex;
	m_Slot = slot;
}
