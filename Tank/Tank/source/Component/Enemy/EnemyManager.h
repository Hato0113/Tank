//------------------------
// 敵マネージャ
//-- author --
//	HatoriMasashi
//------------------------
#pragma once

//-- include --
#include "MySystem\DirectX.h"

enum class EnemyType
{
	Normal,
};

//-- クラス定義 --
class EnemyManager
{
public:
	static void Summon(DirectX::XMINT2 pos, EnemyType type);
	static void Add(uint16_t add = 1);
	static void Kill(uint16_t kill = 1);
	static void Reset() { CurrentEnemy = 0; }
private:
	static int CurrentEnemy;
};
