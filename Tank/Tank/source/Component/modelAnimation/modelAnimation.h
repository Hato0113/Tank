//------------------------------
//	モデルアニメーションクラス(課題用)
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Model\Model.h"

//-- クラス定義 --
class modelAnimation : public Component
{
public:
	modelAnimation();
	virtual ~modelAnimation() = default;

public:
	void Update() override;

	void SetModel(Model* pModel) { m_pModel = pModel; }

private:
	Model* m_pModel;

	DirectX::XMFLOAT3 m_rotDest;
	DirectX::XMFLOAT3 m_rotModel;
	int m_AnimStack;
	double m_AnimTime;

};
