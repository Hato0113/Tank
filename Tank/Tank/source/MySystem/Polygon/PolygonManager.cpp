//-----------------------------
//	2Dポリゴンマネージャー
//-- author --
//	HatoriMasashi
//-----------------------------

//-- include --
#include "PolygonManager.h"
#include "Allocator\Allocator.hpp"
#include "MySystem\DirectX.h"
#include "Shader\ShaderManager.h"
#include "Component\Polygon\Polygon.h"
#include "Shader\ConstantBufferStruct.h"

namespace
{
	ID3D11SamplerState* s_pSampler;

	ID3D11Buffer* s_VertexBuf;

	//-- 行列情報 --
	DirectX::XMFLOAT4X4 s_mView;
	DirectX::XMFLOAT4X4 s_mProj;
	DirectX::XMFLOAT4X4 s_mTex;
	DirectX::XMFLOAT4X4 s_mWorld;

	const int vertexNum = 4;
	struct PolygonVertexInfo
	{
		DirectX::XMFLOAT3 Vertex;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT4 Diffuse;
	}s_VertexInfo[vertexNum];
}

void PolygonManager::Init()
{
	using namespace DirectX;
	//-- シェーダー初期化 --
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

	/*
		2Dポリゴンは頂点情報が一定のため、
		マネージャーが情報をもつ。
		頂点色情報に関してはMap関数でコンポーネントが持つ情報へ書き換える。
	*/
	//-- 頂点バッファ生成 --


	s_VertexInfo[0].Vertex = { -0.5f,0.5f,0.0f };
	s_VertexInfo[1].Vertex = { 0.5f,0.5f,0.0f };
	s_VertexInfo[2].Vertex = { -0.5f,-0.5f,0.0f };
	s_VertexInfo[3].Vertex = { 0.5f,-0.5f,0.0f };

	for (int i = 0; i < vertexNum; i++)
		s_VertexInfo[i].Diffuse = { 1.0f,1.0f,1.0f,1.0f };

	s_VertexInfo[0].UV = { 0.0f,0.0f };
	s_VertexInfo[1].UV = { 1.0f,0.0f };
	s_VertexInfo[2].UV = { 0.0f,1.0f };
	s_VertexInfo[3].UV = { 1.0f,1.0f };

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(s_VertexInfo);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA initData;
	//ZeroMemory(&initData, sizeof(initData));
	//initData.pSysMem = &s_VertexInfo[0];

	hr = pDevice->CreateBuffer(&vbd, nullptr, &s_VertexBuf);
	if (FAILED(hr)) return;

	//-- 行列情報初期化 --
	XMStoreFloat4x4(&s_mWorld,XMMatrixIdentity());
	XMStoreFloat4x4(&s_mView, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&s_mProj, XMMatrixOrthographicLH((float)WindowInfo::m_ScreenWidth, (float)WindowInfo::m_ScreenHeight, 1.0f, 100.0f));
	XMStoreFloat4x4(&s_mTex, XMMatrixIdentity());
	s_mTex._44 = 0.0f;
}

void PolygonManager::Uninit()
{
	if (s_pSampler)
	{
		s_pSampler->Release();
		s_pSampler = nullptr;
	}
	if (s_VertexBuf)
	{
		s_VertexBuf->Release();
		s_VertexBuf = nullptr;
	}
}

void PolygonManager::Draw(CPolygon* polygon)
{
	using namespace DirectX;
	XMFLOAT2 pos = polygon->m_Pos;
	XMMATRIX mWorld = XMMatrixIdentity();
	XMFLOAT2 size = polygon->m_Size;
	XMMATRIX mScale = XMMatrixScaling(size.x, size.y, 1.0f);
	XMMATRIX mRot = XMMatrixRotationZ(polygon->m_AngleZ);
	XMMATRIX mTrans = XMMatrixTranslation(pos.x, pos.y, 0.0f);
	mWorld = XMMatrixMultiply(mWorld, mScale);
	mWorld = XMMatrixMultiply(mWorld, mRot);
	mWorld = XMMatrixMultiply(mWorld, mTrans);
	XMStoreFloat4x4(&s_mWorld, mWorld);

	XMMATRIX mTex = XMMatrixIdentity();
	if (polygon->m_pTex)
	{
		mTex = XMMatrixScaling(polygon->m_FrameSize.x, polygon->m_FrameSize.y, 1.0f);
		mTex *= XMMatrixTranslation(polygon->m_UV.x, polygon->m_UV.y, 0.0f);
		XMStoreFloat4x4(&s_mTex, mTex);
	}
	else
	{
		XMStoreFloat4x4(&s_mTex, mTex);
		s_mTex._44 = 0.0f;
	}

	//-- 頂点情報更新 --
	UpdateVertexBuf(polygon->m_Color, polygon->m_Alpha);

	//-- 定数バッファへの書き込み --
	CB_CameraLight cbCL;
	cbCL.WorldMat = XMMatrixTranspose(mWorld);
	mWorld = XMMatrixIdentity();
	XMMATRIX mView = XMLoadFloat4x4(&s_mView);
	XMMATRIX mProj = XMLoadFloat4x4(&s_mProj);
	cbCL.WVPMat = XMMatrixTranspose(mWorld);
	cbCL.View = XMMatrixTranspose(mView);
	cbCL.Projection = XMMatrixTranspose(mProj);
	cbCL.TextureMat = XMMatrixTranspose(XMLoadFloat4x4(&s_mTex));
	ShaderManager::GetInstance().ConstantWrite("CB_CameraLight", &cbCL);
	//-- シェーダー設定 --
	ShaderManager::GetInstance().BindVS("VS_Polygon");
	ShaderManager::GetInstance().BindPS(polygon->m_useShader.c_str());

	//-- バッファ設定 --
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	UINT stride = sizeof(PolygonVertexInfo);
	UINT offset = 0;
	pDC->IASetVertexBuffers(0, 1, &s_VertexBuf, &stride, &offset);
	pDC->PSSetSamplers(0, 1, &s_pSampler);
	pDC->PSSetShaderResources(0, 1, &polygon->m_pTex);
	pDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pDC->Draw(vertexNum, 0);
}

void PolygonManager::UpdateVertexBuf(DirectX::XMFLOAT3 color, float alpha)
{
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	HRESULT hr = S_OK;

	D3D11_MAPPED_SUBRESOURCE msr;
	hr = pDC->Map(s_VertexBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < vertexNum; i++)
		{
			s_VertexInfo[i].Diffuse = { color.x,color.y,color.z,alpha };
		}
		memcpy_s(msr.pData, sizeof(s_VertexInfo), s_VertexInfo, sizeof(s_VertexInfo));
		pDC->Unmap(s_VertexBuf, 0);
	}
}



