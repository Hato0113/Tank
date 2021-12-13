//------------------------------
//	モデルマネージャー
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "ModelManager.h"
#include "Allocator\Allocator.hpp"
#include "MySystem\DirectX.h"

ModelManager* ModelManager::pModelManager = nullptr;

void ModelManager::Init()
{
	pModelManager = Allocator::Alloc<ModelManager>();
	for (auto obj : pModelManager->m_pModelMap)
	{
		obj.second = nullptr;
	}
}

void ModelManager::Uninit()
{
	for (auto obj : pModelManager->m_pModelMap)
	{
		if (obj.second)
		{
			obj.second->Release();
			delete obj.second;
			obj.second = nullptr;
		}
	}
	Allocator::Free<ModelManager>(pModelManager);
}

void ModelManager::Load(ModelID id, const char* fileName)
{
	if (!pModelManager) return;
	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	pModelManager->m_pModelMap[id] = new CAssimpModel;
	HRESULT hr = pModelManager->m_pModelMap[id]->Load(pDevice, pDC, fileName);
	if (FAILED(hr))
		DX::MakeMessage(L"Model Load Failed");
}

CAssimpModel* ModelManager::Get(ModelID id)
{
	if (!pModelManager) return nullptr;
	return pModelManager->m_pModelMap[id];
}
