//--------------------------
//	弾マネージャ
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once
//-- include --
#include "MySystem\DirectX.h"
#include "Component\Model\Model.h"

enum class BulletType
{
	None,
	Normal,
};

//-- クラス定義 --
class NormalWeapon;
class BulletManager
{
public:
	static void Summon(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir, BulletType type,ModelID modelID = ModelID::ColorBall_Cyan, NormalWeapon* parent = nullptr);
};