//---------------------------
//	エネミー用マップデータ管理
//-- author --
// HatoriMasashi
//---------------------------
#pragma once

//-- include --
#include <DirectXMath.h>

//-- クラス定義 --
class MapManager
{
private:
	MapManager();
	~MapManager();
public:
	static MapManager& GetInstance()
	{
		static MapManager inst;
		return inst;
	}
public:
	struct MapInfo
	{
		bool Block;		//侵入可否
	};
	static DirectX::XMFLOAT3 ConvertWorldPos(DirectX::XMINT2 pos);
	DirectX::XMINT2 SearchTarget(DirectX::XMINT2 start, int len);

private:
	static const DirectX::XMINT2 MapSize;
	static const float BlockSize;
	MapInfo** MapData;

};

