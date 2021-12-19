//--------------------------
//	敵行動コンポーネント
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "EnemyMovement.h"
#include "MySystem\LevelManager\MapManager.h"
#include "MySystem\Resident\ResidentData.h"
#include "MySystem\Resident\ResidentFlag.h"
#include "Function\Primitive\Primitive.h"

EnemyMovement::EnemyMovement()
{
	m_MoveSpeed = ResidentDataManager::GetData().EnemyData.Speed;
	m_MoveLength = 4;
	m_TargetPoint = { 0.0f,0.0f,0.0f };
	m_CurrentMapCoord = { 0,0 };
	m_TargetMapCoord = { -1,-1 };
	m_MoveDir = { 0.0f,0.0f,0.0f };
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
		if (m_TargetMapCoord.x == -1 || m_TargetMapCoord.y == -1) return;
		m_TargetPoint = MapManager::ConvertWorldPos(m_TargetMapCoord);

		//-- debug(ターゲット位置の表示) --
		if (ResidentFlagManager::GetData().GamePlay.Enemy.ShowTarget)
		{
			auto obj = Object::Create("EnemyTarget");
			auto pos = m_TargetPoint;
			pos.y = 11.0f;
			obj->transform->SetPos(pos);
			obj->SetLifeTime(180);
			PrimitiveInfoField info;
			info.m_Color = { 220.0f / 255.0f,20.0f / 255.0f,60.0f / 255.0f,1.0f };
			info.m_Size = { 6.0f,6.0f };
			auto mesh = Primitive::CreateFieldPrimitive(obj, info);
			mesh->SetAlphaFadeTime(180);
			parent->GetScene()->manager->Add(obj);
		}

		m_MoveDir.x = m_TargetPoint.x - pos.x;
		m_MoveDir.z = m_TargetPoint.z - pos.z;
		DirectX::XMVECTOR vec;
		vec = DirectX::XMLoadFloat3(&m_MoveDir);
		vec = DirectX::XMVector3Normalize(vec);
		DirectX::XMStoreFloat3(&m_MoveDir, vec);
	}

	//-- debug(移動禁止) --
	if (!ResidentFlagManager::GetData().GamePlay.Enemy.Move) return;

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