//------------------------------
//	敵生存数UI
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Polygon\Polygon.h"

//-- クラス定義 --
class EnemyTankUI : public Component
{
public:
	EnemyTankUI();
	virtual ~EnemyTankUI() = default;

	void Init() override;
	void Update() override;

private:
	int m_EnemyNum;
	int m_PrevNum;
	Object* m_StrObj;
};
