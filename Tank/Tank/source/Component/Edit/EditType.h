//------------------------
//	エディットタイプ選択
//-- author --
//	HatoriMasashi
//------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Edit\EditPanel.h"

//-- クラス定義 --
class CPolygon;
class EditType : public Component
{
public:
	EditType();
	virtual ~EditType() = default;

	void Update() override;

	void SetStringPolygon(std::vector<CPolygon*> set) { m_StringPolygon = set; }
	void SetPos(DirectX::XMFLOAT2 pos) { m_ScreenPos = pos; }
	void SetSize(DirectX::XMFLOAT2 size) { m_Size = size; }
	void SetType(PanelType type) { m_Type = type; }
	
	static PanelType GetCurrentSelect() { return CurrentSelect; }
private:
	void SetStringActive(bool state);

private:
	std::vector<CPolygon*> m_StringPolygon;
	DirectX::XMFLOAT2 m_ScreenPos;
	DirectX::XMFLOAT2 m_Size;
	PanelType m_Type;
	static PanelType CurrentSelect;
};
