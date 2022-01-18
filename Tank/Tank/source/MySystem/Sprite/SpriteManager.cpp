//------------------------------
//	スプライトマネージャー
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "SpriteManager.h"
#include "Allocator\Allocator.hpp"
#include <fstream>
#include <sstream>
#include "Function\Primitive\Primitive.h"
#include "MySystem\Texture\TextureManager.h"

SpriteManager* SpriteManager::pSpriteManager = nullptr;

void SpriteManager::Init()
{
	pSpriteManager = Allocator::Alloc<SpriteManager>();
	for (auto obj : pSpriteManager->SpriteMap)
	{
		obj.second.clear();
	}
}

void SpriteManager::Uninit()
{
	for (auto sprite : pSpriteManager->SpriteMap)
	{
		for (auto info : sprite.second)
		{
			Allocator::Free(info);
		}
	}
	Allocator::Free(pSpriteManager);
}

std::vector<std::string> split(std::string data, char delimiter)
{
	std::istringstream stream(data);
	std::string field;
	std::vector<std::string> ret;
	while (std::getline(stream, field, delimiter))
	{
		ret.push_back(field);
	}
	return ret;
}


bool SpriteManager::Load(SpriteType type, std::string fileName,TextureID id,int texWidth,int texHeight)
{
	std::ifstream ifs(fileName.c_str());

	std::string lineStr;
	while (std::getline(ifs, lineStr))
	{
		std::vector<std::string> splitData = split(lineStr, ',');
		SpriteInfo* spriteInfo = Allocator::Alloc<SpriteInfo>();
		spriteInfo->name = splitData[0];
		spriteInfo->u = std::stoi(splitData[1]);
		spriteInfo->v = std::stoi(splitData[2]);
		spriteInfo->width = std::stoi(splitData[3]);
		spriteInfo->height = std::stoi(splitData[4]);

		pSpriteManager->SpriteMap[type].push_back(spriteInfo);
	}
	pSpriteManager->TexInfoMap[type].TexID = id;
	pSpriteManager->TexInfoMap[type].Width = texWidth;
	pSpriteManager->TexInfoMap[type].Height = texHeight;

	ifs.close();
	
	return true;
}

Object* SpriteManager::Make(SpriteType type, std::string ItemName,DirectX::XMFLOAT3 pos)
{
	SpriteInfo info;
	bool succes = false;
	for (auto data : pSpriteManager->SpriteMap[type])
	{
		if (ItemName == data->name)
		{
			info = *data;
			succes = true;
			break;
		}
	}
	if (!succes) return nullptr;

	Object* pRet = Object::Create(ItemName.c_str());
	PrimitiveInfo2DObj primitive;
	float width = 1.0f / pSpriteManager->TexInfoMap[type].Width;
	float height = 1.0f / pSpriteManager->TexInfoMap[type].Height;
	float Mag = 0.2f;
	primitive.m_Size = { info.width * Mag,info.height * Mag};
	primitive.m_UV = { width * info.u,height * info.v };
	primitive.m_FrameSize = { width * info.width,height * info.height };
	primitive.m_pTex = TextureManager::Get(pSpriteManager->TexInfoMap[type].TexID);
	Primitive::Create2DObjectPrimitive(pRet, primitive);
	pRet->transform->SetPos({ pos.x,pos.y + primitive.m_Size.y,pos.z });
	return pRet;
}

