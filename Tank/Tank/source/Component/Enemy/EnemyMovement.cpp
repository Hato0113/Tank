//--------------------------
//	敵行動コンポーネント
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "EnemyMovement.h"
#include "MySystem\LevelManager\MapManager.h"
#include "MySystem\Resident\ResidentData.h"

EnemyMovement::EnemyMovement()
{
	m_MoveSpeed = ResidentDataManager::GetData().EnemyData.Speed;
	m_MoveLength = 4;
	m_TargetPoint = { 0.0f,0.0f,0.0f };
	m_CurrentMapCoord = { 0,0 };
	m_TargetMapCoord = { -1,-1 };
}

/*
	初期設定
*/
void EnemyMovement::Setting(DirectX::XMINT2 coord, float speed, int len)
{
	m_MoveSpeed = speed;
	m_MoveLength = len;
	m_CurrentMapCoord = coord;
	parent->transform->SetPos(MapManager::ConvertWorldPos(coord));

}

void EnemyMovement::Update()
{
	auto pos = parent->transform->GetPos();
	//-- ターゲット設定 --
	if (m_TargetMapCoord.x == -1 || m_TargetMapCoord.y == -1)
	{
		m_TargetMapCoord = MapManager::GetInstance().SearchTarget(m_CurrentMapCoord, m_MoveLength);
		m_TargetPoint = MapManager::ConvertWorldPos(m_TargetMapCoord);
		m_MoveDir.x = m_TargetPoint.x - pos.x;
		m_MoveDir.z = m_TargetPoint.z - pos.z;
		DirectX::XMVECTOR vec;
		vec = DirectX::XMLoadFloat3(&m_MoveDir);
		vec = DirectX::XMVector3Normalize(vec);
		DirectX::XMStoreFloat3(&m_MoveDir, vec);
	}

	//-- 目標地点への移動 --
	pos.x += m_MoveDir.x * m_MoveSpeed;
	pos.z += m_MoveDir.z * m_MoveSpeed;
	//超えたかの判定
	if (((m_TargetPoint.x - pos.x) * m_MoveDir.x) <= 0 &&
		((m_TargetPoint.z - pos.z) * m_MoveDir.z <= 0))
	{
		pos = m_TargetPoint;
		m_CurrentMapCoord = m_TargetMapCoord;
		m_TargetMapCoord = { -1,-1 };
	}
	parent->transform->SetPos(pos);

	Rot();
}

void EnemyMovement::Rot()
{
	//-- 方向ベクトルへ近づける --
	Transform* pTrans = parent->transform;

	EulerAngles CurrentAngle = pTrans->GetEulerAngle();

	Quaternion CurrentQuaternion;
	CurrentQuaternion.SetQuaternion(CurrentAngle);

	//-- 目標角度へのクォータニオン --
	Quaternion Target;	//実際のターゲット
	Target.Identity();
	DirectX::XMFLOAT3 dir;
	dir = m_MoveDir;
	float theta = atan2f(dir.x, dir.z);	//目標
	Target.SetToRotateAxisAngle(Quaternion::Up, theta);
	//-- 現在の向きを更新 --
	CurrentQuaternion = Quaternion::Slerp(CurrentQuaternion, Target, 0.05f);
	CurrentAngle.SetEulerAngles(CurrentQuaternion);
	pTrans->SetRotate(CurrentAngle);

}