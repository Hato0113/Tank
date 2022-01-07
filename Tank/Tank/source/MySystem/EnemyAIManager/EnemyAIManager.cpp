//----------------------
//	敵AIマネージャ
//-- author --
//	HatoriMasashi
//----------------------

//-- include --
#include "EnemyAIManager.h"
#include "Scene\SceneManager.h"
#include "Component\Enemy\EnemyAI\EnemyTaskManager.h"
#include "MySystem\Texture\TextureManager.h"
#include "IMGUI\GUI_Message.h"

/*
	コンストラクタ
*/
EnemyAIManager::EnemyAIManager()
{

}

/*
	初期化
*/
void EnemyAIManager::Init()
{
	//ゲームシーンでなければスキップ
	if (SceneManager::GetInstance().GetCurrentSceneType() != SceneType::Game) return;
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	{	//プレイヤー情報取得
		auto player = scene->manager->FindObjectWithTag("Player");
		MiniMapObjectInfo info;
		info.ColliderType = MiniMapObjectInfo::eColliderType::Circle;
		info.Size = 5.0f;
		auto pos = player->transform->GetPos();
		info.Pos = { pos.x,pos.z };
		info.Type = "Player";
		info.object = player;
		AddList(info);
	}
	{	//敵情報取得
		auto enemyList = scene->manager->FindObjectsWithTag("Enemy");
		int enemyID = 0;
		for (auto obj : enemyList)
		{
			MiniMapObjectInfo info;
			auto pos = obj->transform->GetPos();
			info.Pos = { pos.x,pos.z };
			info.ColliderType = MiniMapObjectInfo::eColliderType::Circle;
			info.Size = 5.0f;
			info.EnemyID = enemyID;
			info.Type = "Enemy";
			info.object = obj;
			AddList(info);
			auto enemyTaskManager = obj->GetComponent<EnemyTaskManager>();
			if (enemyTaskManager)
			{
				enemyTaskManager->SetID(enemyID);	//ID付与
			}
			enemyID++;
		}
	}
	{	//マップオブジェクト情報を描画するポリゴン
		auto obj = Object::Create("minimapObj");
		auto back = obj->AddComponent<CPolygon>();
		back->SetSize({ 300.0f,200.0f });
		back->SetPos({ 0.0f,40.0f });
		back->SetAlpha(0.6f);
		back->SetColor({ 0.0f,0.0f,0.0f });

		for (auto& mapObj : m_MapObjectList)
		{
			auto poly = obj->AddComponent<CPolygon>();
			mapObj.polygon = poly;
			poly->SetPos(mapObj.Pos);
			poly->SetLayer(Layer::Front2D);
			poly->SetSize({ 16.0f,16.0f });
			if (mapObj.Type == "Player")
			{
				poly->SetTex(TextureManager::Get(TextureID::minimap_Player));
			}
			else if (mapObj.Type == "Enemy")
			{
				poly->SetTex(TextureManager::Get(TextureID::minimap_Enemy));
			}
			else if (mapObj.Type == "Wall")
			{
				poly->SetTex(TextureManager::Get(TextureID::minimap_Block));
			}
			else if (mapObj.Type == "Niedle")
			{
				poly->SetTex(TextureManager::Get(TextureID::minimap_Niedle));
			}
		}
		scene->manager->Add(obj);
	}
}

/*
	更新処理
*/
void EnemyAIManager::Update()
{
	//ゲームシーンでなければスキップ
	if (SceneManager::GetInstance().GetCurrentSceneType() != SceneType::Game) return;
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	//-- 盤面情報の更新 --
	if (true) {	//敵情報更新
		auto enemyList = scene->manager->FindObjectsWithTag("Enemy");
		for (auto itr = m_MapObjectList.begin(); itr != m_MapObjectList.end();)
		{
			if (itr->Type == "Enemy")
			{
				bool flg = false;	//適合したか否か
				for (auto enemy : enemyList)
				{
					auto id = enemy->GetComponent<EnemyTaskManager>()->GetID();
					if (itr->EnemyID == id)
					{
						auto pos = itr->object->transform->GetPos();
						itr->Pos = { pos.x,pos.z };
						itr->polygon->SetPos(itr->Pos);
						flg = true;
						break;
					}
				}
				if (flg)
				{
					itr++;
				}
				else
				{
					itr->polygon->SetState(false);	//ポリゴン削除
					itr = m_MapObjectList.erase(itr);	//削除
					IG::MessageManager::DrawSystemLog("enemyDeleted");
				}
			}
			else
			{
				itr++;
			}
		}
	}
	if (true) {	//プレイヤー情報更新
		for (auto itr = m_MapObjectList.begin(); itr != m_MapObjectList.end(); itr++)
		{
			if (itr->Type == "Player")
			{
				if (itr->object->GetState())
				{
					auto pos = itr->object->transform->GetPos();
					itr->Pos = { pos.x,pos.z };
					itr->polygon->SetPos(itr->Pos);
				}
				else
				{
					itr->polygon->SetState(false);
					m_MapObjectList.erase(itr);	//非アクティブで削除
					break;
				}
			}
		}
	}
}

/*
	現在の情報から次のタスクを与える
	param : タスクのタイプ
	param : 敵ID
	return : メッセージ
*/
std::string EnemyAIManager::InquireNextTask(std::string TaskType, int id)
{
	if (TaskType == "Movement")
	{
		//-- 次に動く座標を検索 --
		return "none";
	}
	else if (TaskType == "Weapon")
	{
		//-- 攻撃できるか否かを判定 --
		return JudgeShot(id) ? "shot_ok" : "shot_ng";
	}
	else return "none";	//メッセージ無し
}

/*
	射撃できるか否かの判定
	param : 敵ID
	return : 可能か否か
*/
bool EnemyAIManager::JudgeShot(int id)
{
	//-- 情報取得 --
	auto enemyList = GetMapObjectListWithType("Enemy");
	MiniMapObjectInfo enemyInfo;
	for (auto obj : enemyList)
	{
		if (obj.EnemyID == id)
		{
			enemyInfo = obj;
			break;
		}
	}
	MiniMapObjectInfo playerInfo = GetMapObjectListWithType("Player")[0];

	//-- レイ生成 --
	const float increaseRate = (playerInfo.Pos.y - enemyInfo.Pos.y) / (playerInfo.Pos.x - enemyInfo.Pos.x);
	const float constantNum = playerInfo.Pos.y - playerInfo.Pos.x * increaseRate;
	auto rayJudge = [increaseRate, constantNum](DirectX::XMFLOAT2 point)
	{
		return (point.y < increaseRate * point.x + constantNum) ? 1 : 0;
	};
	//範囲チェック true 範囲外 false 範囲内
	auto rangeCheck = [playerInfo, enemyInfo](DirectX::XMFLOAT2 point)
	{
		bool judge = false;
		DirectX::XMFLOAT2 smallpos;
		DirectX::XMFLOAT2 bigpos;
		smallpos.x = (enemyInfo.Pos.x > playerInfo.Pos.x) ? playerInfo.Pos.x : enemyInfo.Pos.x;
		bigpos.x = (enemyInfo.Pos.x > playerInfo.Pos.x) ? enemyInfo.Pos.x : playerInfo.Pos.x;
		smallpos.y = (enemyInfo.Pos.y > playerInfo.Pos.y) ? playerInfo.Pos.y : enemyInfo.Pos.y;
		bigpos.y = (enemyInfo.Pos.y > playerInfo.Pos.y) ? enemyInfo.Pos.y : playerInfo.Pos.y;

		if (smallpos.x > point.x || bigpos.x < point.x)
		{
			judge = true;
		}
		if (smallpos.y > point.y || bigpos.y < point.y)
		{
			judge = true;
		}
		return judge;
	};
	//-- 判定 --
	bool hit = false;
	auto wallList = GetMapObjectListWithType("Wall");
	for (auto& obj : wallList)
	{
		//4頂点生成
		DirectX::XMFLOAT2 point[4];
		point[0] = { obj.Pos.x + obj.Size,obj.Pos.y + obj.Size };
		point[1] = { obj.Pos.x + obj.Size,obj.Pos.y - obj.Size };
		point[2] = { obj.Pos.x - obj.Size,obj.Pos.y + obj.Size };
		point[3] = { obj.Pos.x - obj.Size,obj.Pos.y - obj.Size };
		int judge = 0;
		for (int i = 0; i < 4; i++)
		{
			//-- 範囲チェック --
			if (rangeCheck(point[0]))
				continue;
			
			judge += rayJudge(point[i]);	//判定
		}
		if (judge % 4)
		{
			//当たっている
			hit = true;
			break;
		}
	}

	return !hit;
}

/*
	オブジェクト登録
	param : オブジェクト情報
*/
void EnemyAIManager::AddList(MiniMapObjectInfo info)
{
	m_MapObjectList.push_back(info);
}

/*
	マップオブジェクトリストから情報取得
	param : オブジェクトタイプ
	return : オブジェクトのvector
*/
std::vector<EnemyAIManager::MiniMapObjectInfo> EnemyAIManager::GetMapObjectListWithType(std::string type)
{
	std::vector<MiniMapObjectInfo> list;
	for (auto& obj : m_MapObjectList)
	{
		if (obj.Type == type)
			list.push_back(obj);
	}

	return list;
}


