//-----------------------------
//	メッシュコンポーネント
//-- author --
//	HatoriMasashi
//-----------------------------

//-- include --
#include "Mesh.h"

Mesh::Mesh()
{
	DirectX::XMStoreFloat4x4(&m_mtxTexture, DirectX::XMMatrixIdentity());
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_NumIndex = 0;
	m_NumVertex = 0;
	pTexture = nullptr;
	m_PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_Material.Reset();

	//-- フラグ初期化 --
	DrawFlag = true;
	useLight = true;
	Component::SetLayer(Layer::Back3D);

	//-- メッシュ操作変数 --
	m_AlphaFadeTime = -1;
	m_AlphaFadeMax = 0;
}

Mesh::~Mesh()
{
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}
}

void Mesh::Update()
{
	if (m_AlphaFadeTime > 0)
	{
		m_AlphaFadeTime--;
		auto mat = GetMaterial();
		mat.Diffuse.w = static_cast<float>(m_AlphaFadeTime) / m_AlphaFadeMax;
		SetMaterial(mat);
	}
}

void Mesh::Draw()
{
	if (!DrawFlag) return;

	MeshManager::Draw(this);
}

void Mesh::SetAlphaFadeTime(int time)
{
	m_AlphaFadeMax = m_AlphaFadeTime = time;
}