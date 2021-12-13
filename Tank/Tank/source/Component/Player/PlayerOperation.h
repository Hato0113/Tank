//--------------------------
//	プレイヤー操作
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Collider\BoxCollider.h"
#include "Component\Collider\SphereCollider.h"

//-- クラス定義 --
class PlayerOperation : public Component
{
public:
	PlayerOperation();
	virtual ~PlayerOperation() = default;

public:
	void Update() override;

private:
	void Move();
	void Rot();

public:
	void SetSize(float size);
	void OnCollisionStay(Collider* col) override;

	//-- メンバ変数 --
private:

	//移動関連
	DirectX::XMFLOAT3 m_Move;
	DirectX::XMFLOAT3 m_Acc;	//ラジアン値の加速度 -PI/2 ~ PI/2
	DirectX::XMFLOAT3 m_InputDir;	//入力方向
	float m_CurrentTheta;	//現在の向き
	bool m_IsInput = false;	//そのフレームで更新があったか否か

	//判定用
	BoxCollider* m_pYCollider;		//Y判定用
	SphereCollider* m_pXZCollider;	//XZ判定用
	DirectX::XMFLOAT3 m_PrevPos;
	float m_Size;
	bool m_StateXZ;
	DirectX::XMINT3 m_MoveBanFlag;
	DirectX::XMINT2 m_PrevHitXZ;	//1F前に当たっていたか否か
};

