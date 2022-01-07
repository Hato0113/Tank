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
	Normal,	//普通
	Quick,	//ちょっと速い
	Rapid,	//ちょっと射撃が速い
	Strong,	//強い
};

//-- クラス定義 --
class EnemyManager
{
public:
	static void Summon(DirectX::XMINT2 pos, EnemyType type);
	static void Add(uint16_t add = 1);
	static bool Kill(uint16_t kill = 1);
	static void Reset() { CurrentEnemy = 0; }
private:
	static int CurrentEnemy;
};
