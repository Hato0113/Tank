//--------------------------
//	スロープ判定クラス
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"

//-- クラス定義 --
class SlopeCollider : public Component
{
public:
	SlopeCollider();
	virtual ~SlopeCollider() = default;

public:
	void SetDirection(DirectX::XMFLOAT3 dir) { m_Direction = dir; }
	DirectX::XMFLOAT3 GetDirection() { return m_Direction; }
	//-- メンバ変数 --
private:
	DirectX::XMFLOAT3 m_Direction;	//向き(垂直面が奥)
	
};

