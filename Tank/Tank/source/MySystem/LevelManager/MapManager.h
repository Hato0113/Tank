//---------------------------
//	エネミー用マップデータ管理
//-- author --
// HatoriMasashi
//---------------------------
#pragma once

//-- include --
#include <DirectXMath.h>
#include "Objects\Object.h"

//-- enum --
enum class MapObjectType
{
	Wall,
	Hole,
};

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
	struct MapInfo	//１マスが持つ情報
	{
		bool Block;		//侵入可否
	};
	static DirectX::XMFLOAT3 ConvertWorldPos(DirectX::XMINT2 pos);	//マップ上座標からの変換
	DirectX::XMINT2 SearchTarget(DirectX::XMINT2 start, int len);	//敵用 移動ターゲット検索
	Object* CreateMapObject(DirectX::XMINT2 pos, MapObjectType type);	//マップオブジェクト生成

private:
	static const DirectX::XMINT2 MapSize;
	static const float BlockSize;
	MapInfo** MapData;

};

