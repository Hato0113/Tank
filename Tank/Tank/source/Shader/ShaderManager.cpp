//-----------------------------
//	シェーダーマネージャー
//-- author --
//	HatoriMasashi
//-----------------------------

//-- include --
#include "ShaderManager.h"
#include "MySystem\DirectX.h"
#include "ConstantBufferStruct.h"
#include "Allocator\Allocator.hpp"
#include "Vertex\PolygonVertex.h"
#include "Vertex\MeshVertex.h"
#include "Vertex\AssimpVertex.h"
#include "MySystem\Texture\TextureManager.h"
#include "System\Texture.h"

ShaderManager::ShaderManager()
{
	//none
}

/*
	定数バッファへの書き込み
*/
void ShaderManager::ConstantWrite(std::string name, void* data)
{
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	pDC->UpdateSubresource(ConstantMap[name]->m_Buffer, 0, nullptr, data, 0, 0);
}

/*
	シェーダー初期化
	使用するシェーダーをここで初期化
*/
void ShaderManager::Init()
{
	HRESULT hr = S_OK;
	//-- 頂点シェーダー --
	auto vsPolygon = Allocator::Alloc<PolygonVertex>();
	hr = vsPolygon->Make("data/shader/PolygonVS.cso");
	Set("VS_Polygon", vsPolygon);

	auto vsMesh = Allocator::Alloc<MeshVertex>();
	hr = vsMesh->Make("data/shader/MeshVS.cso");
	Set("VS_Mesh", vsMesh);

	auto vsAssimp = Allocator::Alloc<AssimpVertex>();
	hr = vsAssimp->Make("data/shader/AssimpVS.cso");
	Set("VS_Assimp", vsAssimp);

	//-- ピクセルシェーダー --
	auto psMesh = Allocator::Alloc<PixelBase>();
	hr = psMesh->Make("data/shader/MeshPS.cso");
	Set("PS_Mesh", psMesh);

	auto psPolygon = Allocator::Alloc<PixelBase>();
	hr = psPolygon->Make("data/shader/PolygonPS.cso");
	Set("PS_Polygon", psPolygon);

	auto psAssimp = Allocator::Alloc<PixelBase>();
	hr = psAssimp->Make("data/shader/AssimpPS.cso");
	Set("PS_Assimp", psAssimp);

	auto psAssimpBump = Allocator::Alloc<PixelBase>();
	hr = psAssimpBump->Make("data/shader/AssimpBumpPS.cso");
	Set("PS_AssimpBump", psAssimpBump);

	//-- 定数バッファ --
	//カメラ,ライト
	ConstantBuffer* cbCameraLight = Allocator::Alloc<ConstantBuffer>();
	cbCameraLight->Make(sizeof(CB_CameraLight), 0, ConstantType::Both);
	Set("CB_CameraLight", cbCameraLight);

	//Assimpモデルマテリアル
	auto cbAssimpMaterial = Allocator::Alloc<ConstantBuffer>();
	cbAssimpMaterial->Make(sizeof(CB_Material), 1, ConstantType::Pixel);
	Set("CB_Material", cbAssimpMaterial);

	//ボーン情報
	auto cbBone = Allocator::Alloc<ConstantBuffer>();
	cbBone->Make(sizeof(CB_AssimpBone), 2, ConstantType::Vertex);
	Set("CB_AssimpBone", cbBone);

	//-- テクスチャ --
	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();
	auto normalTexTest = Allocator::Alloc<ShaderTexture>();
	hr = CreateTextureFromFile(pDevice, "data/texture/ShaderResource/LargeRockNormal.png", &normalTexTest->m_Tex);
	normalTexTest->Set(normalTexTest->m_Tex, 4);
	Set("TX_LargeRock", normalTexTest);

	//-- バッファのバインド --
	ConstantBufferBind();
}

/*
	定数バッファ / テクスチャのバインド
*/
void ShaderManager::ConstantBufferBind()
{
	auto dc = DX::DirectXManager::GetInstance().GetDeviceContext();
	//-- 定数バッファバインド --
	for (auto obj : ConstantMap)
	{
		obj.second->Bind();
	}

	//-- テクスチャバインド --
	//ここでは固定で使用するテクスチャを入れる(モデルごとに設定した方がよい?)
	//for (auto obj : TextureMap)
	//{
	//	dc->PSSetShaderResources(obj.second->m_Slot, 1, &obj.second->m_Tex);
	//}
}

/*
	テクスチャのバインド
*/
void ShaderManager::TextureResourceBind()
{
	auto dc = DX::DirectXManager::GetInstance().GetDeviceContext();
	for (auto obj : TextureMap)
	{
		dc->PSSetShaderResources(obj.second->m_Slot, 1, &obj.second->m_Tex);
	}
}

/*
	シェーダーの解放 / 定数バッファ解放
*/
void ShaderManager::Uninit()
{
	for (auto obj : VertexMap)
	{
		Allocator::Free(obj.second);
		obj.second = nullptr;
	}

	for (auto obj : PixelMap)
	{
		Allocator::Free(obj.second);
		obj.second = nullptr;
	}

	for (auto obj : ConstantMap)
	{
		Allocator::Free(obj.second);
		obj.second = nullptr;
	}

	for (auto obj : TextureMap)
	{
		obj.second->m_Tex->Release();
		Allocator::Free(obj.second);
		obj.second = nullptr;
	}

	VertexMap.clear();
	PixelMap.clear();
	ConstantMap.clear();
	TextureMap.clear();
}

/*
	頂点シェーダーセッティング
*/
void ShaderManager::Set(std::string name, VertexBase* ptr)
{
	VertexMap.insert(std::make_pair(name, ptr));
}

/*
	ピクセルシェーダーセッティング
*/
void ShaderManager::Set(std::string name, PixelBase* ptr)
{
	PixelMap.insert(std::make_pair(name, ptr));
}

/*
	定数バッファセッティング
*/
void ShaderManager::Set(std::string name, ConstantBuffer* ptr)
{
	ConstantMap.insert(std::make_pair(name, ptr));
}

/*
	テクスチャセッティング
*/
void ShaderManager::Set(std::string name, ShaderTexture* ptr)
{
	TextureMap.insert(std::make_pair(name, ptr));
}

/*
	頂点シェーダーバインド
*/
void ShaderManager::BindVS(std::string name)
{
	VertexMap[name]->Bind();
}

/*
	ピクセルシェーダーバインド
*/
void ShaderManager::BindPS(std::string name)
{
	PixelMap[name]->Bind();
}