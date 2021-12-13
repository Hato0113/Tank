//--------------------------
//	カメラ
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "MySystem\DirectX.h"

//-- クラス定義 --
class Camera : public Component
{
public:
	Camera();
	virtual ~Camera() = default;

	void Update() override;

	void SettingInfo(float ratio, float fov, float nearClip, float farClip);
	void SettingTransform(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target);

	DirectX::XMFLOAT4X4 GetViewMat() { return m_ViewMatrix; };
	DirectX::XMFLOAT4X4 GetProjMat() { return m_ProjectionMatrix; }

	float GetNear() { return m_NearClip; }
	float GetFar() { return m_FarClip; }

	DirectX::XMFLOAT3 GetTarget() { return m_Target; }
	DirectX::XMFLOAT3 GetDir() { return m_DirVec; }

	float GetFov() { return m_Fov; }
	float GetNClip() { return m_NearClip; }
	float GetFClip() { return m_FarClip; }
	float GetAspect() { return m_AspectRatio; }

private:
	void CalcWorldMat();
	void UpdateVPMat();

	void Move();	//debug : カメラ移動
	void Rotate();	//debug : 回転


	//-- メンバ変数 --
private:
	DirectX::XMFLOAT3 m_Target;
	DirectX::XMFLOAT3 m_UpVec;
	DirectX::XMFLOAT3 m_DirVec;

	float m_AspectRatio;
	float m_Fov;
	float m_NearClip;
	float m_FarClip;

	DirectX::XMFLOAT4X4 m_WorldMatrix;
	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;
};
