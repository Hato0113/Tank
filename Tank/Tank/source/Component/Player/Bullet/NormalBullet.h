//--------------------------
//	通常弾
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "BulletBase.h"
#include "BulletManager.h"

//-- クラス定義 --
class NormalBullet : public BulletBase
{
	friend class BulletManager;
public:
	NormalBullet();
	virtual ~NormalBullet() = default;
	void OnCollisionEnter(Collider*) override;

public:
	void Update() override;

private:
	int m_HitCount;
};
