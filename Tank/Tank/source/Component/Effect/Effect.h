//--------------------------
//	エフェクトコンポーネント
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "MySystem\Effect\EffectManager.h"
#include "Component\Component.h"

//-- クラス定義 --
class Effect : public Component
{
	friend class EffectManager;
public:
	Effect();
	virtual ~Effect() = default;

public:
	void Init();
	void SetEffect(Effekseer::EffectRef effect);
	void SetMaxTime(int time) { m_MaxTime = time; }
	void SetAutoPlay(bool state) { m_AutoPlay = state; }
	void SetScale(float mag) { SetScale({ mag,mag,mag }); }
	void SetScale(DirectX::XMFLOAT3 scale);

private:
	Effekseer::EffectRef m_Effect;
	Effekseer::Handle m_Handle;
	Effekseer::ManagerRef m_Manager;
	bool m_AutoPlay;
	int m_CurrentTime;
	int m_MaxTime;
};

