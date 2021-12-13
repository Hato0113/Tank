//-----------------------------
//	定数バッファ生成
//-- author --
//	HatoriMasashi
//-----------------------------

//-- include --
#include "ConstantBuffer.h"
#include "MySystem\DirectX.h"

ConstantBuffer::ConstantBuffer()
{
	m_Buffer = nullptr;
	m_Slot = 0;
	m_Type = ConstantType::max;
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_Buffer)
	{
		m_Buffer->Release();
		m_Buffer = nullptr;
	}
}

/*
	自身のバインド
*/
void ConstantBuffer::Bind()
{
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	switch (m_Type)
	{
	case ConstantType::Vertex:
		pDC->VSSetConstantBuffers(m_Slot, 1, &m_Buffer);
		break;
	case ConstantType::Pixel:
		pDC->PSSetConstantBuffers(m_Slot, 1, &m_Buffer);
		break;
	case ConstantType::Both:
		pDC->VSSetConstantBuffers(m_Slot, 1, &m_Buffer);
		pDC->PSSetConstantBuffers(m_Slot, 1, &m_Buffer);
		break;
	case ConstantType::max:
	default:
		break;
	}
}

/*
	バッファ生成
	引数 : サイズ,登録スロット,バインドタイプ
*/
HRESULT ConstantBuffer::Make(size_t size, int slot, ConstantType type)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = static_cast<UINT>(size);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();
	hr = pDevice->CreateBuffer(&bufferDesc, nullptr, &m_Buffer);
	if (FAILED(hr))
		return hr;
	//-- 情報保持 --
	m_Type = type;
	m_Slot = slot;

	return hr;
}
