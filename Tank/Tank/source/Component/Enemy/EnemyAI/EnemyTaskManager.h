//--------------------------
//	敵AI管理コンポーネント
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "EnemyTaskBase.h"
#include <vector>

//-- クラス定義 --
class EnemyTaskManager : public Component
{
public:
	EnemyTaskManager();
	virtual ~EnemyTaskManager() = default;

	void Update() override;

	int GetID() { return m_EnemyID; }
	void SetID(int id) { m_EnemyID = id; }

	void AddTask(EnemyTaskBase* task) { m_EnemyTasks.push_back(task); }

private:
	std::vector<EnemyTaskBase*> m_EnemyTasks;	//自身が持つタスクリスト
	int m_EnemyID;	//マネージャーから与えられたID
};
