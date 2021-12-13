//------------------------------
//	モデルアニメーションクラス(課題用)
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "modelAnimation.h"
using namespace DirectX;
#include "MySystem\KeyInput\KeyInput.h"
#include <time.h>

//-- define --
#define ANIM_IDLE			(0)
#define ANIM_JUMP			(1)
#define ANIM_KICK			(2)
#define ANIM_RUN			(3)
#define ANIM_SLASH			(4)
#define ANIM_WALK			(5)

#define	PLAYER_RADIUS		(50.0f)							// 半径
#define	VALUE_MOVE_PLAYER	(0.155f)						// 移動速度
#define	RATE_MOVE_PLAYER	(0.025f)						// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(0.079f)							// 回転速度(Degree)
#define	RATE_ROTATE_PLAYER	(0.10f)							// 回転慣性係数

modelAnimation::modelAnimation()
{
	m_pModel = nullptr;
	m_rotDest = { 0.0f,0.0f,0.0f };
	m_rotModel = { 0.0f,0.0f,0.0f };
	m_AnimStack = 0;
	m_AnimTime = 0;
}

void modelAnimation::Update()
{
	XMMATRIX mtxWorld, mtxRot;

	// カメラの向き取得
	XMFLOAT3 rotCamera = XMFLOAT3(0, 0, 0);//GetCameraAngle();
	bool isInput = false;

	if (KeyInput::GetKeyPress('A')) {
		isInput = true;
		if (KeyInput::GetKeyPress('W')) {
			m_rotDest.y = rotCamera.y + XM_PI + XM_PI * 0.25f;
		}
		else if (KeyInput::GetKeyPress('S')) {
			m_rotDest.y = rotCamera.y + XM_PI * 2 - XM_PI / 4.0f;
		}
		else {
			m_rotDest.y = rotCamera.y - XM_PIDIV2;
		}
	}
	else if (KeyInput::GetKeyPress('D')) {
		isInput = true;
		if (KeyInput::GetKeyPress('W')) {
			m_rotDest.y = rotCamera.y + XM_PI * 0.75f;
		}
		else if (KeyInput::GetKeyPress('S')) {
			m_rotDest.y = rotCamera.y + XM_PI * 0.25f;
		}
		else {
			m_rotDest.y = rotCamera.y + XM_PIDIV2;
		}
	}
	else if (KeyInput::GetKeyPress('W')) {
		isInput = true;
		m_rotDest.y = XM_PI + rotCamera.y;
	}
	else if (KeyInput::GetKeyPress('S')) {
		isInput = true;
		m_rotDest.y = rotCamera.y;
	}

	if (KeyInput::GetKeyPress('J')) {
		// 左回転
		m_rotDest.y -= VALUE_ROTATE_PLAYER;
		if (m_rotDest.y < -XM_PI) {
			m_rotDest.y += XM_2PI;
		}
	}
	if (KeyInput::GetKeyPress('L')) {
		// 右回転
		m_rotDest.y += VALUE_ROTATE_PLAYER;
		if (m_rotDest.y > XM_PI) {
			m_rotDest.y -= XM_2PI;
		}
	}

	switch (m_AnimStack) {
	case ANIM_IDLE:
		if (KeyInput::GetKeyPush('X')) {
			m_AnimStack = ANIM_SLASH;
			m_AnimTime = 0.0;
		}
		if (KeyInput::GetKeyPush('C')) {
			m_AnimStack = ANIM_KICK;
			m_AnimTime = 0.0;
		}
		// THRU
	case ANIM_WALK:
	case ANIM_RUN:
		if (KeyInput::GetKeyPush('Z')) {
			m_AnimStack = ANIM_JUMP;
			m_AnimTime = 0.0;
		}
		break;
	}

	// 目的の角度までの差分
	float fDiffRotY = m_rotDest.y - m_rotModel.y;
	if (fDiffRotY > XM_PI) {
		fDiffRotY -= XM_2PI;
	}
	if (fDiffRotY < -XM_PI) {
		fDiffRotY += XM_2PI;
	}

	// 目的の角度まで慣性をかける
	m_rotModel.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (m_rotModel.y > XM_PI) {
		m_rotModel.y -= XM_2PI;
	}
	if (m_rotModel.y < -XM_PI) {
		m_rotModel.y += XM_2PI;
	}

	// アニメーション更新
	float fSpeed = (float)static_cast<int>(isInput);
	if (KeyInput::GetKeyPress(VK_LSHIFT) && isInput)
	{
		fSpeed *= 4.0f;
	}
	int nStack = ANIM_IDLE;
	if (fSpeed > 3.0f)
		nStack = ANIM_RUN;
	else if (fSpeed > 0.2f)
		nStack = ANIM_WALK;
	switch (m_AnimStack) {
	case ANIM_IDLE:
	case ANIM_WALK:
	case ANIM_RUN:
		if (m_AnimStack != nStack) {
			m_AnimStack = nStack;
			m_AnimTime = 0.0;
		}
		break;
	}
	static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	double dNowTime = clock() / double(CLOCKS_PER_SEC);
	double dSlice = dNowTime - dLastTime;
	dLastTime = dNowTime;
	m_AnimTime += dSlice;
	if (m_AnimTime >= m_pModel->GetModel()->GetAnimDuration(m_AnimStack)) {
		switch (m_AnimStack) {
		case ANIM_JUMP:
		case ANIM_SLASH:
		case ANIM_KICK:
			m_AnimStack = nStack;
			break;
		}
		m_AnimTime = 0.0;
	}

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(m_rotModel.x, m_rotModel.y, m_rotModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// ワールドマトリックス設定
	EulerAngles angle;
	//angle.m_Bank = m_rotModel.x;
	//angle.m_Pitch = m_rotModel.z;
	angle.m_Heading = m_rotModel.y;
	parent->transform->SetRotate(angle);

	//-- モデル情報更新 --
	CAssimpModel* pModel = m_pModel->GetModel();
	pModel->SetAnimIndex(m_AnimStack);
	pModel->SetAnimTime(m_AnimTime);
}