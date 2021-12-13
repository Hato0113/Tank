//-------------------------
//	デバッグポーズ
//-- author --
//	HatoriMasashi
//-------------------------
#pragma once

//-- include --
#include "Component\Component.h"

//-- クラス定義 --
class DebugPause : public Component
{
public:
	DebugPause();
	virtual ~DebugPause() = default;

public:
	void Update() override;

	bool m_IsPausing;
};

