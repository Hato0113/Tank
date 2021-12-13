//---------------------------
//	エネミー用マップデータ管理
//-- author --
// HatoriMasashi
//---------------------------

//-- include --
#include "MapManager.h"
#include <vector>

//-- 静的メンバ --
const DirectX::XMINT2 MapManager::MapSize = { 15,10 };
const float MapManager::BlockSize = 20.0f;

MapManager::MapManager()
{
	MapData = new MapInfo*[MapSize.y];
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
	const DirectX::XMINT2 dirIndex[dirMax] = {{0,1},{1,0},{0,-1},{-1,0}};	//探索用

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
		//移動先を保存
		moveRoot.push_back(next);
		current = next;
	}

	//-- 最終目標 --
	DirectX::XMINT2 ret{0,0};
	if (moveRoot.size() != len + 1)
		return ret;
	else
	{
		ret = moveRoot[len];
		return ret;
	}
	return ret;
}