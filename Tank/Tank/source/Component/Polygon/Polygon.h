//-----------------------------
//	ポリゴンコンポーネント
//-- author --
//	HatoriMasashi
//-----------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "MySystem\Polygon\PolygonManager.h"

//-- クラス定義 --
class CPolygon : public Component
{
	friend class PolygonManager;
public:
	CPolygon();
	virtual ~CPolygon();

	void Draw() override;
	//-- セット関数 --
public:
	void SetPos(DirectX::XMFLOAT2 pos) { m_Pos = pos; }
	void SetSize(DirectX::XMFLOAT2 size) { m_Size = size; }
	void SetUV(DirectX::XMFLOAT2 uv) { m_UV = uv; }
	void SetFrameSize(DirectX::XMFLOAT2 frame) { m_FrameSize = frame; }
	void SetColor(DirectX::XMFLOAT3 color) { m_Color = color; }
	void SetAlpha(float a) { m_Alpha = a; }
	void SetAngleZ(float angle) { m_AngleZ = angle; }
	void SetTex(ID3D11ShaderResourceView* ptex) { m_pTex = ptex; }

	//-- メンバ変数 --
private:
	DirectX::XMFLOAT2 m_Pos;	//3Dの座標は使えないので別で持たせる。
	DirectX::XMFLOAT2 m_Size;
	DirectX::XMFLOAT2 m_UV;
	DirectX::XMFLOAT2 m_FrameSize;
	DirectX::XMFLOAT3 m_Color;
	float m_AngleZ;
	float m_Alpha;
	ID3D11ShaderResourceView* m_pTex;
};
