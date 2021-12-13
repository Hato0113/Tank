//--------------------------
//	弾ベースクラス
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Collider\SphereCollider.h"
#include "Component\Model\Model.h"


//-- クラス定義 --
class NormalWeapon;
class BulletBase : public Component
{
public:
	BulletBase() = default;
	virtual ~BulletBase();

public:
	virtual void Update();
	void SetSphereCollider(SphereCollider* col) { m_pSC = col; }
	virtual void OnCollisionEnter(Collider*);	//当たった瞬間
	virtual void OnCollisionStay(Collider*) {};	//当たっている間
	virtual void OnCollisionExit(Collider*) {};	//当たらなくなった瞬間
	void SetModel(Model* p) { m_pModel = p; }
	void RotateModel(float spd);
	void SetInvalidTime(int time) { m_InvalidTime = time; }
	void SetParentWeapon(NormalWeapon* parentWeapon) { m_ParentWeapon = parentWeapon; }

protected:
	DirectX::XMFLOAT3 m_PrevPos;
	DirectX::XMFLOAT3 m_Dir;
	float m_Speed;
	SphereCollider* m_pSC;
	Model* m_pModel;
	int m_InvalidTime;
	NormalWeapon* m_ParentWeapon;
};

