//--------------------------
//	プレイヤー操作
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "PlayerOperation.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "Component\Collider\SlopeCollider.h"
#include <algorithm>
#include "IMGUI\GUI_Message.h"
#include "Function\screen\Screen.h"
#include "MySystem\Resident\ResidentData.h"

PlayerOperation::PlayerOperation()
{
	m_Move = { 0.0f,0.0f,0.0f };
	m_Acc = { 0.0f,0.0f,0.0f };

	m_pXZCollider = nullptr;
	m_pYCollider = nullptr;
	m_PrevPos = { 0.0f,0.0f,0.0f };
	m_StateXZ = true;
	m_MoveBanFlag = { 0,0,0 };
}

void PlayerOperation::Update()
{
	m_PrevPos = parent->transform->GetPos();
	Move();
	Rot();

	DirectX::XMFLOAT2 scpos = Screen::GetScreenPos(parent->transform->GetPos());
	//IG::MessageManager::DrawLog("scPos : %.3f / %.3f", scpos.x, scpos.y);

	//debug　プレイヤーの方向ベクトル
	EulerAngles ea = parent->transform->GetEulerAngle();
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, ea.GetRotateMatrix());
	//IG::MessageManager::DrawLog("Forward : %f / %f / %f", mat._31, mat._32, mat._33);
	//IG::MessageManager::DrawLog("Right : %f / %f / %f", mat._11, mat._12, mat._13);
	//IG::MessageManager::DrawLog("Up : %f / %f / %f", mat._21, mat._22, mat._23);

}

void PlayerOperation::Move()
{
	if (KeyInput::GetKeyPress(VK_RBUTTON)) return;

	const float Acc = DirectX::XM_PI / 2 / ResidentDataManager::GetData().PlayerData.AccFrame;	//加速3F
	const float MaxSpeed = ResidentDataManager::GetData().PlayerData.MaxSpeed;	//最大速度
	const float Dec = DirectX::XM_PI / 2 / ResidentDataManager::GetData().PlayerData.DecFrame;	//減速5F

	//-- 移動処理 --
	m_IsInput = false;
	if (KeyInput::GetKeyPress('W'))
	{
		m_Acc.z += Acc;
		m_IsInput = true;
	}
	if (KeyInput::GetKeyPress('S'))
	{
		m_Acc.z -= Acc;
		m_IsInput = true;
	}
	if (KeyInput::GetKeyPress('A'))
	{
		m_Acc.x -= Acc;
		m_IsInput = true;
	}
	if (KeyInput::GetKeyPress('D'))
	{
		m_Acc.x += Acc;
		m_IsInput = true;
	}

	//-- 速度上限 --
	if (fabsf(m_Acc.z) > DirectX::XM_PI / 2)
		m_Acc.z = DirectX::XM_PI / 2 * (int)(fabsf(m_Acc.z) / m_Acc.z);
	if (fabsf(m_Acc.x) > DirectX::XM_PI / 2)
		m_Acc.x = DirectX::XM_PI / 2 * (int)(fabsf(m_Acc.x) / m_Acc.x);

	//-- 減速処理 --
	if (!KeyInput::GetKeyPress('W') &&
		!KeyInput::GetKeyPress('S') &&
		m_Acc.z != 0.0f)
	{
		int temp = (int)(fabsf(m_Acc.z) / m_Acc.z);		//符号のみ
		if (temp > 0)
			m_Acc.z -= Dec;
		else if (temp < 0)
			m_Acc.z += Dec;
		if (temp != (int)(fabsf(m_Acc.z) / m_Acc.z))
			m_Acc.z = 0.0f;		//符号がひっくり返ったら0
	}
	if (!KeyInput::GetKeyPress('A') &&
		!KeyInput::GetKeyPress('D') &&
		m_Acc.x != 0.0f)
	{
		int temp = (int)(fabsf(m_Acc.x) / m_Acc.x);
		if (temp > 0)
			m_Acc.x -= Dec;
		else if (temp < 0)
			m_Acc.x += Dec;
		if (temp != (int)(fabsf(m_Acc.x) / m_Acc.x))
			m_Acc.x = 0.0f;
	}
	//-- 斜め方向が速くならないように --
	DirectX::XMVECTOR AccVec;
	AccVec = DirectX::XMLoadFloat3(&m_Acc);
	AccVec = DirectX::XMVector3Normalize(AccVec);
	DirectX::XMFLOAT3 NormalizedAcc;
	DirectX::XMStoreFloat3(&NormalizedAcc, AccVec);

	//-- ダッシュ (debug) --
	int Dash = 1;
	if (KeyInput::GetKeyPress(VK_LSHIFT))
		Dash = 2;
	m_Move.x = m_Acc.x * Dash * MaxSpeed * fabsf(NormalizedAcc.x) * !(m_MoveBanFlag.x);
	m_Move.z = m_Acc.z * Dash * MaxSpeed * fabsf(NormalizedAcc.z) * !(m_MoveBanFlag.z);

	//-- 入力方向算出 --
	DirectX::XMVECTOR dirvec;
	dirvec = DirectX::XMLoadFloat3(&m_Move);
	dirvec = DirectX::XMVector3Normalize(dirvec);
	DirectX::XMFLOAT3 len;
	DirectX::XMStoreFloat3(&len, DirectX::XMVector3Length(dirvec));
	if (len.x != 0.0f)
	{
		DirectX::XMStoreFloat3(&m_InputDir, dirvec);
	}
	//IG::MessageManager::DrawLog("playerdir : %.3f / %.3f / %.3f", m_InputDir.x, m_InputDir.y, m_InputDir.z);

	parent->transform->AddPos(m_Move);	//座標更新

	//範囲判定
	auto pos = parent->transform->GetPos();
	constexpr float Radius = 6.0f;
	constexpr DirectX::XMFLOAT3 BorderMax = { 150.0f - Radius,0.0f,140.0f - Radius };
	constexpr DirectX::XMFLOAT3 BorderMin = { -150.0f + Radius,0.0f,-60.0f + Radius };

	if (pos.x > BorderMax.x)
		pos.x = BorderMax.x;
	if (pos.x < BorderMin.x)
		pos.x = BorderMin.x;
	if (pos.z > BorderMax.z)
		pos.z = BorderMax.z;
	if (pos.z < BorderMin.z)
		pos.z = BorderMin.z;
	parent->transform->SetPos(pos);
}

void PlayerOperation::SetSize(float size)
{
	m_Size = size;
	BoxCollider* pBC = parent->AddComponent<BoxCollider>();
	pBC->SetSize({ 0.1f,100.0f,0.1f });
	pBC->SetSubjectState(true);
	m_pYCollider = pBC;
	SphereCollider* pSC = parent->AddComponent<SphereCollider>();
	pSC->SetRadius(size);
	pSC->SetSubjectState(false);
	m_pXZCollider = pSC;
}

void PlayerOperation::Rot()
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
	dir = m_InputDir;
	float theta = atan2f(dir.x, dir.z) - DirectX::XM_PIDIV2;	//目標
	IG::MessageManager::DrawLog("theta : %.6f", theta);
	Target.SetToRotateAxisAngle(Quaternion::Up, theta);
	//-- 現在の向きを更新 --
	if (m_IsInput)
	{
		CurrentQuaternion = Quaternion::Slerp(CurrentQuaternion, Target, 0.05f);
		CurrentAngle.SetEulerAngles(CurrentQuaternion);
		pTrans->SetRotate(CurrentAngle);

	}
}

void PlayerOperation::OnCollisionEnter(Collider* col)
{
	if (col->parent->transform->GetName() == "MapObject")
	{
		parent->transform->SetPos(m_PrevPos);
	}
}

void PlayerOperation::OnCollisionStay(Collider* col)
{
	if (col->parent->transform->GetName() == "MapObject")
	{
		parent->transform->SetPos(m_PrevPos);
	}
}