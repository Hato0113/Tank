//-------------------------
//	敵AIベース
//-- author --
//	HatoriMasashi
//-------------------------
#pragma once

//-- include --
#include "Component\Component.h"

//-- クラス定義 --
class EnemyTaskBase abstract : public Component
{
public:
	EnemyTaskBase();
	virtual ~EnemyTaskBase() = default;
public:
	virtual void Init() {};
	virtual void Update() {};

	bool CheckEnd();
	std::string GetType() { return TaskType; }

	void SetRemainTime(int time) { m_RemainTime = time; }

protected:
	bool m_End;				//終わっているか否か
	int m_RemainTime;		//タスク残り時間
	std::string TaskType;	//タスクタイプ
};
