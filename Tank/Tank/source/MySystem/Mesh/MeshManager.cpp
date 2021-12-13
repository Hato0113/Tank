//-----------------------------
//	メッシュマネージャー
//-- author --
//	HatoriMasashi
//-----------------------------

//-- include --
#include "MeshManager.h"
#include "Allocator\Allocator.hpp"
#include "MySystem\DirectX.h"
#include "Component\Camera\Camera.h"
#include "Component/Light/DirectionalLight.h"
#include "Component\Mesh\Mesh.h"
#include "Shader\ShaderManager.h"
#include "Shader\ConstantBufferStruct.h"

namespace
{
	ID3D11SamplerState* s_pSampler;
}

void MeshManager::Init()
{
	HRESULT hr = S_OK;

	//-- テクスチャサンプラ生成 --
	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, &s_pSampler);
	if (FAILED(hr)) {
		return;
	}
}

void MeshManager::Uninit()
{
	if (s_pSampler)
	{
		s_pSampler->Release();
		s_pSampler = nullptr;
	}
}

void MeshManager::Draw(Mesh* pMesh)
{
	using namespace DirectX;
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	Object* pCamObj = pMesh->parent->GetScene()->manager->FindObjectWithName("MainCamera");
	Camera* pCamera = nullptr;
	if (pCamObj)
	{
		pCamera = pCamObj->GetComponent<Camera>();
		if (!pCamera)
		{
			DX::MakeMessage(L"Camera not found.");
			return;
		}
	}
	Object* pLightObj = pMesh->parent->GetScene()->manager->FindObjectWithName("Light");
	DirectionalLight* pLight = nullptr;
	if (pLightObj)
	{
		pLight = pLightObj->GetComponent<DirectionalLight>();
		if (!pLight)
		{
			DX::MakeMessage(L"Light not found.");
			return;
		}
	}

	//-- ワールドマトリクス作成 --
	Transform* trans = pMesh->parent->transform;
	XMMATRIX mWorld = XMMatrixIdentity();
	XMFLOAT3 scale = trans->GetScale();
	XMMATRIX mScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX mRot = trans->GetEulerAngle().GetRotateMatrix();
	XMFLOAT3 pos = trans->GetPos();
	XMMATRIX mTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	mWorld = XMMatrixMultiply(mWorld, mScale);
	mWorld = XMMatrixMultiply(mWorld, mRot);
	mWorld = XMMatrixMultiply(mWorld, mTrans);

	//-- テクスチャマトリクス更新 --

	//-- 定数バッファへの書き込み --
	if (!pMesh->GetLightUse())
		pLight->m_Enable = false;
	CB_CameraLight cb0;
	cb0.WorldMat = XMMatrixTranspose(mWorld);
	cb0.View = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMat()));
	cb0.Projection = XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetProjMat()));
	cb0.TextureMat = XMMatrixTranspose(XMLoadFloat4x4(&pMesh->GetTexMat()));
	cb0.EyePos = XMLoadFloat3(&pCamera->parent->transform->GetPos());
	cb0.LightDir = XMLoadFloat3(&pLight->GetDir());
	cb0.LightDiffuse = XMLoadFloat4(&pLight->GetDiffuse());
	cb0.LightAmbient = XMLoadFloat4(&pLight->GetAmbient());
	cb0.LightSpecular = XMLoadFloat4(&pLight->GetSpecular());
	MATERIAL mat = pMesh->GetMaterial();
	CB_Material cb1;
	cb1.Diffuse = XMLoadFloat4(&mat.Diffuse);
	cb1.Ambient = DirectX::XMVectorSet(mat.Ambient.x, mat.Ambient.y, mat.Ambient.z, pMesh->HasTexture());
	cb1.Emissive = XMLoadFloat4(&mat.Emissive);
	cb1.Specular = DirectX::XMVectorSet(mat.Specular.x, mat.Specular.y, mat.Specular.z, mat.Power);
	ShaderManager::GetInstance().ConstantWrite("CB_CameraLight", &cb0);
	ShaderManager::GetInstance().ConstantWrite("CB_Material", &cb1);
	//-- シェーダー設定 --
	ShaderManager::GetInstance().BindVS("VS_Mesh");
	ShaderManager::GetInstance().BindPS("PS_Mesh");
	if (!pLight->m_Enable)
		pLight->m_Enable = true;

	//-- 頂点バッファ / インデックスバッファセット --
	UINT stride = sizeof(MeshVertexInfo);
	UINT offset = 0;
	pDC->IASetVertexBuffers(0, 1, pMesh->GetVertexBufpp(), &stride, &offset);
	pDC->IASetIndexBuffer(pMesh->GetIndexBuf(), DXGI_FORMAT_R32_UINT, 0);

	pDC->PSSetSamplers(0, 1, &s_pSampler);
	pDC->PSSetShaderResources(0, 1, pMesh->GetTexpp());
	pDC->IASetPrimitiveTopology(pMesh->GetPrimitiveType());

	//-- 描画コール --
	pDC->DrawIndexed(pMesh->GetNumIndex(), 0, 0);
}

HRESULT MeshManager::MakeMeshInfo(Mesh* pMesh, MeshVertexInfo vertexWk[], int indexWk[])
{

	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(MeshVertexInfo) * pMesh->GetNumVertex();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertexWk;
	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, pMesh->GetVertexBufpp());
	if (FAILED(hr)) {
		return hr;
	}

	CD3D11_BUFFER_DESC ibd(pMesh->GetNumIndex() * sizeof(int), D3D11_BIND_INDEX_BUFFER);
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indexWk;
	hr = pDevice->CreateBuffer(&ibd, &initData, pMesh->GetIndexBufpp());

	return hr;
}