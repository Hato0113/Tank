//--------------------------
//	敵AI管理コンポーネント
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "EnemyTaskManager.h"
#include "MySystem\EnemyAIManager\EnemyAIManager.h"
#include "Component\Enemy\EnemyAI\EnemyTaskWeapon.h"

/*
	コンストラクタ
*/
EnemyTaskManager::EnemyTaskManager()
{
	m_EnemyTasks.clear();
}

/*
	更新処理
*/
void EnemyTaskManager::Update()
{
	//-- 現在のタスクの更新処理 --
	for (auto &com : m_EnemyTasks)
	{
		if (com->CheckEnd())
		{
			//-- タスク終了->新しいタスクの割振り --
			auto message = EnemyAIManager::GetInstance().InquireNextTask(com->GetType(), m_EnemyID);
			if (message != "none")
			{

			}
			if (message == "shot_ok")
			{
				auto wep = dynamic_cast<EnemyTaskWeapon*>(com);
				if (wep)
				{
					wep->SetCanShot(true);
					wep->SetRemainTime(2);
				}
			}
			else if (message == "shot_ng")
			{
				auto wep = dynamic_cast<EnemyTaskWeapon*>(com);
				if (wep)
				{
					wep->SetCanShot(false);
					wep->SetRemainTime(2);
				}
			}
		}
	}
}
