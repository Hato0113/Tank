//-----------------------------
//	Assimp用頂点シェーダー
//-- author --
//	HatoriMasashi
//-----------------------------

//-- include --
#define _CRT_SECURE_NO_WARNINGS
#include "AssimpVertex.h"
#include "MySystem\DirectX.h"

HRESULT AssimpVertex::Make(std::string fileName)
{
	HRESULT hr = E_FAIL;
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (!fp) { return hr; }

	//-- ファイルの中身をメモリに読み込み --
	char* pData = nullptr;
	//-- ファイルのデータサイズを調べる --
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);	//移動量検出
	fseek(fp, 0, SEEK_SET);
	//-- メモリ確保して読み込み --
	pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();

	hr = pDevice->CreateVertexShader(pData, fileSize, nullptr, &m_Shader);
	if (FAILED(hr))
	{
		if (pData) delete[] pData;
		return hr;
	}

	const D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_INDEX",  0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = pDevice->CreateInputLayout(layout, _countof(layout), pData, fileSize, &m_Layout);

	if (pData) delete[] pData;

	return hr;
}