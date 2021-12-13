//---------------------------
//	ゲームスタート時タイムライン
//-- author --
// HatoriMasashi
//---------------------------
#pragma once

//-- include --
#include "TimelineBase.h"

//-- クラス定義 --
class GameStartTL :public TimelineBase
{
public:
	GameStartTL();
	virtual ~GameStartTL() = default;

public:
	void Update() override;
};

