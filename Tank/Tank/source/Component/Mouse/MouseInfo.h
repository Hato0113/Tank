//------------------
// マウス情報
//-- author --
//	HatoriMasashi
//------------------
#pragma once

//-- include --
#include "Component\Component.h"

//-- クラス定義 --
class MouseInfo : public Component
{
public:
	MouseInfo();
	~MouseInfo() = default;

public:
	void Update() override;
	DirectX::XMVECTOR GetMouseVec();
	DirectX::XMFLOAT2 GetMousePos() { return m_MousePos; }

private:
	static DirectX::XMFLOAT2 m_MousePos;	//マウスローカルスクリーン座標
};
