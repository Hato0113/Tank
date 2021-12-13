//-----------------------------
//	2Dポリゴンマネージャー
//-- author --
//	HatoriMasashi
//-----------------------------
#pragma once

//-- include --
#include <DirectXMath.h>

//-- クラス定義 --
class CPolygon;
class PolygonManager
{
public:
	static void Init();
	static void Uninit();
	static void Draw(CPolygon*);
private:
	static void UpdateVertexBuf(DirectX::XMFLOAT3 color, float alpha);
};
