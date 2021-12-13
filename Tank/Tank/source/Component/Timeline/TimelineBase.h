//---------------------------
//	タイムラインベース
//-- author --
// HatoriMasashi
//---------------------------
#pragma once

//-- include --
#include "Component\Component.h"

//-- クラス定義 --
class TimelineBase : public Component
{
public:
	TimelineBase();
	virtual ~TimelineBase() = default;

public:
	virtual void Update() {};

protected:
	int m_CurrentTime;	//現在時間
};
