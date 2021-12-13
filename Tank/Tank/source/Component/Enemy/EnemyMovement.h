//--------------------------
//	敵行動コンポーネント
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "MySystem\Resident\ResidentData.h"

//-- クラス定義 --
class EnemyMovement : public Component
{
public:
	EnemyMovement();
	virtual ~EnemyMovement() = default;

	void Update();
	void Rot();
	void Setting(DirectX::XMINT2 coord,float speed = ResidentDataManager::GetData().EnemyData.Speed,int len = 4);

public:
	float m_MoveSpeed;
	int m_MoveLength;	//移動距離(マス)
	DirectX::XMFLOAT3 m_TargetPoint;	//目標座標
	DirectX::XMFLOAT3 m_MoveDir;
	DirectX::XMINT2 m_CurrentMapCoord;	//現在のマップ位置
	DirectX::XMINT2 m_TargetMapCoord;	//目標のマップ位置
};
