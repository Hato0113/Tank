//--------------------------
//	カメラ
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Camera.h"
#include "MySystem\main.h"
#include "MySystem\DirectX.h"
#include "MySystem\KeyInput\KeyInput.h"

Camera::Camera()
{
	m_Target = { 0.0f,0.0f,0.0f };
	m_UpVec = { 0.0f,1.0f,0.0f };
	m_DirVec = { 0.0f,0.0f,0.0f };

	m_AspectRatio = (float)WindowInfo::m_ScreenWidth / WindowInfo::m_ScreenHeight;
	m_Fov = 30.0f;
	m_NearClip = 5.0f;
	m_FarClip = 1000.0f;

	DirectX::XMStoreFloat4x4(&m_WorldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_ViewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, DirectX::XMMatrixIdentity());
}

void Camera::SettingInfo(float ratio, float fov, float nearClip, float FarClip)
{
	m_AspectRatio = ratio;
	m_Fov = fov;
	m_NearClip = nearClip;
	m_FarClip = FarClip;
}

void Camera::SettingTransform(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target)
{
	parent->transform->SetPos(pos);
	m_Target = target;
}

void Camera::Update()
{
	if (KeyInput::GetKeyPress(VK_RBUTTON))
	{
		Move();
		Rotate();
	}

	CalcWorldMat();
	UpdateVPMat();
}

void Camera::CalcWorldMat()
{
	using namespace DirectX;
	XMFLOAT3 target = m_Target;
	XMFLOAT3 pos = parent->transform->GetPos();

	XMVECTOR vecZ = XMVectorSet(target.x - pos.x, target.y - pos.y, target.z - pos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_UpVec);
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	m_WorldMatrix._11 = vX.x;
	m_WorldMatrix._12 = vX.y;
	m_WorldMatrix._13 = vX.z;
	m_WorldMatrix._14 = 0.0f;
	m_WorldMatrix._21 = vY.x;
	m_WorldMatrix._22 = vY.y;
	m_WorldMatrix._23 = vY.z;
	m_WorldMatrix._24 = 0.0f;
	m_WorldMatrix._31 = vZ.x;
	m_WorldMatrix._32 = vZ.y;
	m_WorldMatrix._33 = vZ.z;
	m_WorldMatrix._34 = 0.0f;
	m_WorldMatrix._41 = pos.x;
	m_WorldMatrix._42 = pos.y;
	m_WorldMatrix._43 = pos.z;
	m_WorldMatrix._44 = 1.0f;
}

void Camera::UpdateVPMat()
{
	using namespace DirectX;
	XMFLOAT3 pos = parent->transform->GetPos();

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixLookAtLH(
		XMLoadFloat3(&pos), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_UpVec)));
	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip));

	//-- カメラベクトル再計算 --
	m_DirVec = XMFLOAT3(m_WorldMatrix._31, m_WorldMatrix._32, m_WorldMatrix._33);
}

void Camera::Move()
{
	//-- 変数宣言 --
	using namespace DirectX;
	static XMFLOAT3 CameraMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
	const float DefaultMax = 5.0f;
	static float MaxSpeed = DefaultMax;
	XMINT3 InputFlag = XMINT3(0, 0, 0);

	//-- カメラの方向に対する三軸決定 --
	XMVECTOR dirVec;
	XMFLOAT3 pos = parent->transform->GetPos();
	dirVec = XMLoadFloat3(&m_Target) - XMLoadFloat3(&pos);
	XMVECTOR rightVec = XMVector3Cross(dirVec, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMVECTOR upVec = XMVector3Cross(rightVec, dirVec);

	//-- 差分保存 --
	XMFLOAT3 relative;
	relative.x = m_Target.x - pos.x;
	relative.y = m_Target.y - pos.y;
	relative.z = m_Target.z - pos.z;

	//-- 移動量反映 --
	if (KeyInput::GetKeyPress('D'))	//右
	{
		CameraMove.x -= MaxSpeed * 0.25f;
		InputFlag.x = 1;
	}
	if (KeyInput::GetKeyPress('A'))	//左
	{
		CameraMove.x += MaxSpeed * 0.25f;
		InputFlag.x = 1;
	}
	if (KeyInput::GetKeyPress('W'))	//前
	{
		CameraMove.z += MaxSpeed * 0.25f;
		InputFlag.y = 1;
	}
	if (KeyInput::GetKeyPress('S'))	//後ろ
	{
		CameraMove.z -= MaxSpeed * 0.25f;
		InputFlag.y = 1;
	}
	if (KeyInput::GetKeyPress('E'))	//上昇
	{
		CameraMove.y += MaxSpeed * 0.25f;
		InputFlag.z = 1;
	}
	if (KeyInput::GetKeyPress('Q'))	//下降
	{
		CameraMove.y -= MaxSpeed * 0.25f;
		InputFlag.z = 1;
	}
	if (KeyInput::GetKeyPress(VK_LSHIFT))
	{
		MaxSpeed = DefaultMax * 2;
	}
	else MaxSpeed = DefaultMax;

	//-- 減速処理 --
	const float Dec = 0.8f;
	if (!InputFlag.x)
	{
		if (CameraMove.x != 0.0f)
		{
			CameraMove.x *= Dec;
			if (fabsf(CameraMove.x) < 0.5f)
				CameraMove.x = 0.0f;
		}
	}
	if (!InputFlag.y)
	{
		if (CameraMove.y != 0.0f)
		{
			CameraMove.y *= Dec;
			if (fabsf(CameraMove.y) < 0.5f)
				CameraMove.y = 0.0f;
		}
	}
	if (!InputFlag.z)
	{
		if (CameraMove.z != 0.0f)
		{
			CameraMove.z *= Dec;
			if (fabsf(CameraMove.z) < 0.5f)
				CameraMove.z = 0.0f;
		}
	}

	//--　移動速度上限 --
	if (fabsf(CameraMove.x) > MaxSpeed)
		CameraMove.x = MaxSpeed * CameraMove.x / fabsf(CameraMove.x);
	if (fabsf(CameraMove.y) > MaxSpeed)
		CameraMove.y = MaxSpeed * CameraMove.y / fabsf(CameraMove.y);
	if (fabsf(CameraMove.z) > MaxSpeed)
		CameraMove.z = MaxSpeed * CameraMove.z / fabsf(CameraMove.z);

	//-- 現在座標に移動量を加算 --
	dirVec = XMVector3Normalize(dirVec);
	rightVec = XMVector3Normalize(rightVec);
	upVec = XMVector3Normalize(upVec);
	XMVECTOR currentPos = XMLoadFloat3(&pos);
	currentPos += CameraMove.x * rightVec
		+ CameraMove.y * upVec + CameraMove.z * dirVec;
	XMStoreFloat3(&pos, currentPos);
	parent->transform->SetPos(pos);

	//-- ターゲットの座標を更新 --
	m_Target.x = pos.x + relative.x;
	m_Target.y = pos.y + relative.y;
	m_Target.z = pos.z + relative.z;
}

void Camera::Rotate()
{
	using namespace DirectX;
	//-- 各回転軸の回転行列 --
	XMMATRIX mRotX = DirectX::XMMatrixIdentity();
	XMMATRIX mRotY = DirectX::XMMatrixIdentity();
	XMMATRIX mRotZ = DirectX::XMMatrixIdentity();
	XMFLOAT3 newAngle = { 0.0f,0.0f,0.0f };
	POINT MouseMove = KeyInput::GetMouseMove();
	//-- マウスホイールの動きで視点回転を行う --
	const float PI = 3.1415926535f;
	const float RotXSpeed = PI / (WindowInfo::m_ScreenHeight * 1.5f) * MouseMove.y;
	const float RotYSpeed = PI / (WindowInfo::m_ScreenWidth * 1.5f) * MouseMove.x;
	static float angleY = 0.0f;
	//-- X/Z軸 --
	newAngle.x += sinf((angleY) + PI / 2) * RotXSpeed;
	newAngle.z += cosf((angleY) + PI / 2) * RotXSpeed;
	mRotX = DirectX::XMMatrixRotationX(newAngle.x);
	mRotZ = DirectX::XMMatrixRotationZ(newAngle.z);
	//-- Y軸 --
	angleY += RotYSpeed;
	mRotY = DirectX::XMMatrixRotationY(RotYSpeed);

	//-- 行列演算 --
	XMMATRIX mRotation;
	mRotation = DirectX::XMMatrixMultiply(mRotY, DirectX::XMMatrixMultiply(mRotX, mRotZ));
	XMVECTOR vec;
	XMFLOAT3 pos = parent->transform->GetPos();
	XMFLOAT3 relative;
	relative.x = m_Target.x - pos.x;
	relative.y = m_Target.y - pos.y;
	relative.z = m_Target.z - pos.z;
	vec = XMLoadFloat3(&relative);
	vec = XMVector3TransformCoord(vec, mRotation);
	XMStoreFloat3(&relative, vec);

	//-- ターゲット座標更新 --
	m_Target.x = pos.x + relative.x;
	m_Target.y = pos.y + relative.y;
	m_Target.z = pos.z + relative.z;
}