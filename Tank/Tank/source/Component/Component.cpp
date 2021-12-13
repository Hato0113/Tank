//--------------------------
//	コンポーネントベースクラス
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Component/Component.h"

Component::Component()
{
	parent = nullptr;
	m_Layer = Layer::Default;
	m_State = true;
	m_Active = true;
}