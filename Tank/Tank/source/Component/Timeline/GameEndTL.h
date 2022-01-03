//-----------------------
//	ゲーム終了時TL
//-- author --
//	HatoriMasahi
//-----------------------
#pragma once

//-- include --
#include "TimelineBase.h"

//-- クラス定義 --
class GameEndTL : public TimelineBase
{
public:
	GameEndTL(bool state);	//true : clear / false : failed
	virtual ~GameEndTL() = default;

public:
	void Update() override;
private:
	bool m_State;
};
