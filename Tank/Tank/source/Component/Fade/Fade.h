//--------------------------
//	フェード管理
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Polygon\Polygon.h"

enum class FadeEvent
{
	EventNone,
	Middle,
	End,
};

enum class FadeMode
{
	None,
	FadeIn,
	FadeOut,
	FadeDelete,
};

//-- クラス定義 --
class Fade : public Component
{
public:
	Fade();
	virtual ~Fade() = default;

	//-- メソッド --
public:
	void Init() override;
	void Update() override;
	void SetUp(FadeMode mode);
	FadeEvent GetEventTrigger() { return m_Event; }

	//-- 変数 --
private:
	CPolygon* m_Polygon;
	FadeEvent m_Event;
	FadeMode m_Mode;	//0 : 動きなし / 1 : フェードイン / 2 : フェードアウト
	int m_CurrentTime;
	const int MaxTime = 60;
};

