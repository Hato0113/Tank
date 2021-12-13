//--------------------------
//	武器(装備クラス)
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Weapon.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "Scene\ConstantScene.h"
#include "Component\Mouse\MouseInfo.h"
#include "Function\screen\Screen.h"

WeaponBase::WeaponBase()
{
	m_Mode = WeaponMode::None;
	m_BulletType = BulletType::None;
	m_Dir = { 0.0f,0.0f,1.0f };

	m_Start.Identity();
	m_Target.Identity();
	m_CurrentTheta = 0.0f;
	m_Time = 0.0f;
	m_Ratio = 0.0f;
}

void WeaponBase::UpdateDirSelf()
{
	bool input = false;
	float PrevTheta = m_CurrentTheta;
	//-- 入力部 --
	if (KeyInput::GetKeyPress('I'))
	{
		m_CurrentTheta = 0.0f;
		input = true;
	}
	else if (KeyInput::GetKeyPress('K'))
	{
		m_CurrentTheta = DirectX::XM_PI;
		input = true;
	}
	else if (KeyInput::GetKeyPress('J'))
	{
		m_CurrentTheta = -DirectX::XM_PI / 2.0f;
		input = true;
	}
	else if (KeyInput::GetKeyPress('L'))
	{
		m_CurrentTheta = DirectX::XM_PI / 2.0f;
		input = true;
	}

	//-- 演算部 --
	if (PrevTheta != m_CurrentTheta)
	{
		//-- 変数リセット --
		m_Ratio = 0.0f;
		m_Time = 0.0f;

		//-- 目標変更 --
		m_Target.SetToRotateAxisAngle(Quaternion::Up, m_CurrentTheta);
		m_Start = m_Current;

		//--　現在にたいしてどれぐらい離れているか計算 --
		EulerAngles eaCurrent;
		EulerAngles eaTarget;
		eaCurrent.SetEulerAngles(m_Start);
		eaTarget.SetEulerAngles(m_Target);

		float dif = fabsf(eaCurrent.m_Heading - eaTarget.m_Heading);
		if (dif > DirectX::XM_PI)
			dif = DirectX::XM_2PI - dif;
		dif /= DirectX::XM_PIDIV2;
		m_Ratio = 1.0f / 180.0f * DirectX::XM_PIDIV2 / dif;
	}
	if (input)
	{
		m_Time += m_Ratio;
		m_Current = Quaternion::Slerp(m_Start, m_Target, m_Time);
	}
	else return;	//入力がない場合変更なし
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, m_Current.GetRotateMatrix());
	m_Dir = { mat._31,mat._32,mat._33 };
}

/*
	マウスでのターゲット方向決定
*/
void WeaponBase::UpdateDirByMouse()
{
	//-- マウス座標ゲット --
	DirectX::XMFLOAT2 mousePos = { 0,0 };
	auto mouse = ConstantScene::GetInstance().manager->FindObjectWithName("MouseManager");
	if (mouse)
	{
		auto mouseInfo = mouse->GetComponent<MouseInfo>();
		if (mouseInfo)
			mousePos = mouseInfo->GetMousePos();
	}
	
	//-- マウスへのベクトル算出 --
	DirectX::XMFLOAT2 scpos = Screen::GetScreenPos(parent->transform->GetPos());
	DirectX::XMFLOAT2 dir;
	dir.x = mousePos.x - scpos.x;
	dir.y = mousePos.y - scpos.y;
	DirectX::XMVECTOR vec;
	vec = DirectX::XMLoadFloat2(&dir);
	vec = DirectX::XMVector2Normalize(vec);
	DirectX::XMStoreFloat2(&dir, vec);

	m_Dir.x = dir.x;
	m_Dir.z = -dir.y;


}

void WeaponBase::TargetPlayer()
{
	auto player =  parent->GetScene()->manager->FindObjectWithTag("Player");
	if (!player) return;

	DirectX::XMFLOAT3 playerPos = player->transform->GetPos();
	DirectX::XMFLOAT3 myPos = parent->transform->GetPos();
	DirectX::XMFLOAT3 dir;
	dir.x = playerPos.x - myPos.x;
	dir.y = 0.0f;
	dir.z = playerPos.z - myPos.z;
	DirectX::XMVECTOR vec;
	vec = DirectX::XMLoadFloat3(&dir);
	vec = DirectX::XMVector3Normalize(vec);
	DirectX::XMStoreFloat3(&dir, vec);
	m_Dir = dir;
}