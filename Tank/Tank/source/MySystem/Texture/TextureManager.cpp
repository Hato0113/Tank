//------------------------------
//	テクスチャマネージャー
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "TextureManager.h"
#include "Allocator\Allocator.hpp"
#include "System\Texture.h"
#include "MySystem\DirectX.h"
#include "MySystem\Application.hpp"

TextureManager* TextureManager::pTexManager = nullptr;

void TextureManager::Init()
{
	pTexManager = Allocator::Alloc<TextureManager>();
	for (auto it : pTexManager->m_pTexMap)
	{
		it.second = nullptr;
	}
}

void TextureManager::Uninit()
{
	for (auto it : pTexManager->m_pTexMap)
	{
		if (it.second)
		{
			it.second->Release();
			it.second = nullptr;
		}
	}

	Allocator::Free(pTexManager);
}

void TextureManager::Load(TextureID id, const char* fileName)
{
	if (!tank::Application::GetGameFlag()) return;
	if (!pTexManager) return;
	ID3D11Device* pDevice = DX::DirectXManager::GetInstance().GetDevice();
	HRESULT hr = CreateTextureFromFile(pDevice, fileName, &pTexManager->m_pTexMap[id]);
	if (FAILED(hr))
	{
		DX::MakeMessage(L"Texture Load Failed");
		tank::Application::SetGameFlag(false);
	}
}

ID3D11ShaderResourceView* TextureManager::Get(TextureID id)
{
	if (!pTexManager) return nullptr;
	return pTexManager->m_pTexMap[id];
}


