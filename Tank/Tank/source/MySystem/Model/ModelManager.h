//------------------------------
//	モデルマネージャー
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include <unordered_map>
#include "ModelID.h"
#include "System\AssimpModel.h"

//-- クラス定義 --
class ModelManager
{
public:
	static void Init();
	static void Uninit();
	static void Load(ModelID, const char* fileName);
	static CAssimpModel* Get(ModelID);
private:
	static ModelManager* pModelManager;
	std::unordered_map<ModelID, CAssimpModel*> m_pModelMap;
};
