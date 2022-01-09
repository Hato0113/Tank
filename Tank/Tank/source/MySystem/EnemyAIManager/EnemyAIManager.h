//----------------------
//	敵AIマネージャ
//-- author --
//	HatoriMasashi
//----------------------
#pragma once

//-- include --
#include <string>
#include <vector>
#include "Objects\Object.h"
#include "Component\Polygon\Polygon.h"

//-- クラス定義 --
class EnemyAIManager final
{
private:
	EnemyAIManager();
	~EnemyAIManager() = default;
public:
	static EnemyAIManager& GetInstance()
	{
		static EnemyAIManager inst;
		return inst;
	}

	void Init();
	void Update();
	void Uninit();	//終了処理

	/*
		現在の情報から次のタスクを与える
		param : タスクのタイプ
		param : 敵ID
		return : メッセージ
	*/
	std::string InquireNextTask(std::string TaskType,int id);

	/*
		ミニマップオブジェクト情報
	*/
	struct MiniMapObjectInfo
	{
		DirectX::XMFLOAT2 Pos = { 0.0f,0.0f };	//座標
		std::string Type = "none";	//オブジェクトタイプ
		int EnemyID = 0;	//敵ID
		enum eColliderType
		{
			Square,
			Circle,
		}ColliderType;
		float Size = 5.0f;	//コライダーサイズ
		Object* object = nullptr;	//対応オブジェクトポインタ
		CPolygon* polygon = nullptr;	//描画用ポリゴン
	};
	/*
		オブジェクト登録
		param : オブジェクト情報
	*/
	void AddList(MiniMapObjectInfo info);

private:
	/*
		射撃できるか否かの判定
		param : 敵ID
		return : 可能か否か
	*/
	bool JudgeShot(int id);

	/*
		マップオブジェクトリストから情報取得
		param : オブジェクトタイプ
		return : オブジェクトのvector
	*/
	std::vector<MiniMapObjectInfo> GetMapObjectListWithType(std::string type);

private:
	std::vector<MiniMapObjectInfo> m_MapObjectList;
};
