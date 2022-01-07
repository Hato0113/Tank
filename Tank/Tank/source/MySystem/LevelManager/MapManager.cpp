//---------------------------
//	エネミー用マップデータ管理
//-- author --
// HatoriMasashi
//---------------------------

//-- include --
#include "MapManager.h"
#include <vector>
#include "Component\Model\Model.h"
#include "Component\Collider\BoxCollider.h"
#include "IMGUI\GUI_Message.h"
#include "MySystem\EnemyAIManager\EnemyAIManager.h"

//-- 静的メンバ --
const DirectX::XMINT2 MapManager::MapSize = { 15,10 };
const float MapManager::BlockSize = 20.0f;

MapManager::MapManager()
{
	MapData = new MapInfo * [MapSize.y];
	for (int y = 0; y < MapSize.y; y++)
	{
		MapData[y] = new MapInfo[MapSize.x];
	}

	for (int y = 0; y < MapSize.y; y++)
	{
		for (int x = 0; x < MapSize.x; x++)
		{
			MapData[y][x].Block = false;
		}
	}
}

MapManager::~MapManager()
{
	for (int y = 0; y < MapSize.y; y++)
	{
		delete[] MapData[y];
	}
	delete[] MapData;
}

/*
	配列の座標からワールド座標への変換
*/
DirectX::XMFLOAT3 MapManager::ConvertWorldPos(DirectX::XMINT2 pos)
{
	DirectX::XMFLOAT3 ret;
	ret.x = pos.x * BlockSize - 140.0f;
	ret.z = (MapSize.y - 1 - pos.y) * BlockSize - 50.0f;
	ret.y = 14.0f;
	return ret;
}

/*
	移動目標を探索
*/
DirectX::XMINT2 MapManager::SearchTarget(DirectX::XMINT2 start, int len)
{
	//-- 範囲外なら計算しない --
	if (start.x < 0 || start.x >= MapSize.x ||
		start.y < 0 || start.y >= MapSize.y)
	{
		return { 0,0 };
	}

	const int dirMax = 4;
	const DirectX::XMINT2 dirIndex[dirMax] = { {0,1},{1,0},{0,-1},{-1,0} };	//探索用

	std::vector<DirectX::XMINT2> moveRoot;	//移動したルート保存用
	moveRoot.push_back(start);

	DirectX::XMINT2 current = start;
	int errorNum = 0;	//エラー回数
	for (int i = 0; i < len; i++)
	{
		if (errorNum > 1000)
			break;	//無限ループ対策

		DirectX::XMINT2 next = current;
		int dir = rand() % dirMax;	//方向決定
		next.x += dirIndex[dir].x;
		next.y += dirIndex[dir].y;
		//移動できるか判定
		if (next.x < 0 || next.x >= MapSize.x ||
			next.y < 0 || next.y >= MapSize.y)
		{
			i--;
			errorNum++;
			continue;	//もう一度探索
		}
		//ブロックがあれば移動しない
		if (MapData[next.y][next.x].Block)
		{
			i--;
			errorNum++;
			continue;	//もう一度探索
		}
		//すでに通ったところか判定
		bool flg = false;
		for (auto pos : moveRoot)
		{
			if (pos.x == next.x && pos.y == next.y)
			{
				flg = true;
				break;
			}
		}
		if (flg)
		{
			i--;
			errorNum++;
			continue;
		}
		//斜め移動可能か判定
		flg = false;
		for (auto pos : moveRoot)
		{
			DirectX::XMINT2 temp = pos;
			temp.x += dirIndex[dir].x;
			temp.y += dirIndex[dir].y;
			if (temp.x < 0 || temp.x >= MapSize.x ||
				temp.y < 0 || temp.y >= MapSize.y)
				continue;
			if (MapData[temp.y][temp.x].Block)
			{
				flg = true;
				break;
			}
		}
		if (flg)
		{
			i--;
			errorNum++;
			continue;
		}
		//移動先を保存
		moveRoot.push_back(next);
		current = next;
		//IG::MessageManager::CreateSameMessageFlag = true;
		//IG::MessageManager::DrawLog("pos : %d / %d",next.x,next.y);
		//IG::MessageManager::CreateSameMessageFlag = false;
	}

	//-- 最終目標 --
	DirectX::XMINT2 ret = start;
	if (moveRoot.size() != len + 1)
		return ret;
	else
	{
		ret = moveRoot[len];
		return ret;
	}
	return ret;
}

/*
	マップオブジェクト生成
	引数 : マップ上座標,オブジェクトタイプ
*/
Object* MapManager::CreateMapObject(DirectX::XMINT2 pos, PanelType type)
{
	//-- マップ情報登録 --
	MapData[pos.y][pos.x].Block = true;

	const float WallColor[] =
	{
		1.0f,0.8f,0.6f,0.4f
	};
	const int ColorMax = _countof(WallColor);
	const int Color = rand() % ColorMax;

	//-- オブジェクト生成 --
	auto obj = Object::Create("MapObject");
	switch (type)
	{
	case PanelType::Wall: {
		auto wPos = ConvertWorldPos(pos);
		obj->transform->SetPos(wPos);
		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::WoodBlock));
		model->SetScale(10.0f);
		model->SetDiffuse({ WallColor[Color],WallColor[Color],WallColor[Color],1.0f });
		Quaternion q;
		q.Identity();
		q.SetToRotateAxisAngle(Quaternion::Right, DirectX::XM_PI / 2);
		EulerAngles ea;
		ea.SetEulerAngles(q);
		obj->transform->SetRotate(ea);
		obj->transform->SetTag("MapObject");
		auto col = obj->AddComponent<BoxCollider>();
		col->SetSize(10.0f);
		//-- AIマネージャへの設定 --
		using minimap = EnemyAIManager::MiniMapObjectInfo;
		minimap info;
		info.ColliderType = minimap::eColliderType::Square;
		info.Pos = {wPos.x,wPos.z};
		info.Size = 10.0f;
		info.Type = "Wall";
		EnemyAIManager::GetInstance().AddList(info);
	}
		break;
	case PanelType::Niedle:
	{
		auto summonPos = ConvertWorldPos(pos);
		summonPos.y -= 5.0f;
		obj->transform->SetPos(summonPos);
		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::Niedle));
		model->SetScale(10.0f);
		Quaternion q;
		q.Identity();
		q.SetToRotateAxisAngle(Quaternion::Right, DirectX::XM_PI / 2);
		EulerAngles ea;
		ea.SetEulerAngles(q);
		obj->transform->SetRotate(ea);
		obj->transform->SetTag("MapObjectNoHit");
		auto col = obj->AddComponent<BoxCollider>();
		col->SetSize(10.0f);
		//-- AIマネージャへの設定 --
		using minimap = EnemyAIManager::MiniMapObjectInfo;
		minimap info;
		info.ColliderType = minimap::eColliderType::Square;
		info.Pos = { summonPos.x,summonPos.z };
		info.Size = 10.0f;
		info.Type = "Niedle";
		EnemyAIManager::GetInstance().AddList(info);
	}
		break;
	default:
		break;
	}

	return obj;
}
