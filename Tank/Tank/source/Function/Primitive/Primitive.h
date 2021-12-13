//----------------------------------
//	プリミティブ形状
//-- author --
//	HatoriMasahi
//----------------------------------
#pragma once

//-- include --
#include "MySystem\DirectX.h"
#include "Component\Mesh\Mesh.h"
#include "Objects\Object.h"

/*
	プリミティブクラス
	オブジェクト作成時に形状を持たせる。
	中心座標は親のTransformを参照する。
	作成時に親オブジェクトに対しMeshコンポーネントを追加し、
	形状 / サイズ等を設定する。
*/

//-- クラス定義 --
/*
	線メッシュ情報
*/
class Primitive;
class PrimitiveInfoLine
{
	friend class Primitive;
public:
	PrimitiveInfoLine()	//コンストラクタ
	{
		m_startPos = { 0.0f,0.0f,0.0f };
		m_endPos = { 0.0f,0.0f,0.0f };
		m_startDif = { 1.0f,1.0f,1.0f,1.0f };
		m_endDif = { 1.0f,1.0f,1.0f,1.0f };
	}

	//-- メンバ変数 --
	DirectX::XMFLOAT3 m_startPos;	//描画開始座標
	DirectX::XMFLOAT3 m_endPos;	//描画終了座標
	DirectX::XMFLOAT4 m_startDif;	//描画開始色
	DirectX::XMFLOAT4 m_endDif;	//描画終了色
private:
	int IndexNum = 2;
};
class PrimitiveInfoWireCube
{
	friend class Primitive;
public:
	PrimitiveInfoWireCube()
	{
		m_Size = { 1.0f,1.0f,1.0f };
		m_Dif = { 1.0f,1.0f,1.0f,1.0f };
	}

	//-- メンバ変数 --
	DirectX::XMFLOAT3 m_Size;
	DirectX::XMFLOAT4 m_Dif;
private:
	int IndexNum = 4 * 3 * 2;
	int VertexNum = 8;
};
class PrimitiveInfoCube
{
	friend class Primitive;
public:
	PrimitiveInfoCube()
	{
		m_Size = { 1.0f,1.0f,1.0f };
		m_Dif = { 1.0f,1.0f,1.0f,1.0f };
	}
	//-- メンバ変数 --
	DirectX::XMFLOAT3 m_Size;
	DirectX::XMFLOAT4 m_Dif;
private:
	int IndexNum = 36;
	int VertexNum = 24;
};
class PrimitiveInfoSlope
{
	friend class Primitive;
public:
	PrimitiveInfoSlope()
	{
		m_Size = { 1.0f,1.0f,1.0f };
		m_Dif = { 1.0f,1.0f,1.0f,1.0f };
	}
	//-- メンバ変数 --
	DirectX::XMFLOAT3 m_Size;
	DirectX::XMFLOAT4 m_Dif;
};
class PrimitiveInfoField
{
	friend class Primitive;
public:
	PrimitiveInfoField()
	{
		m_Size = { 10.0f,10.0f };
		m_pTex = nullptr;
		m_Split = { 1,1 };
	}
	//-- メンバ変数 --
	DirectX::XMFLOAT2 m_Size;
	ID3D11ShaderResourceView* m_pTex;
	DirectX::XMINT2 m_Split;
private:
	int m_IndexNum = 4;
	int m_VertexNum = 4;
};
class PrimitiveInfo2DObj
{
	friend class Primitive;
public:
	PrimitiveInfo2DObj()
	{
		m_Size = { 10.0f, 10.0f };
		m_pTex = nullptr;
		m_UV = { 0.0f,0.0f };
		m_FrameSize = { 0.0f,0.0f };
	}
	//-- メンバ変数 --
	DirectX::XMFLOAT2 m_Size;
	ID3D11ShaderResourceView* m_pTex;
	DirectX::XMFLOAT2 m_UV;
	DirectX::XMFLOAT2 m_FrameSize;
};
class PrimitiveInfoSphere
{
	friend class Primitive;
public:
	PrimitiveInfoSphere()
	{
		m_nSlice = 5;
		m_nStack = 5;
		m_fRadius = 20.0f;
		m_Dif = { 1.0f,1.0f,1.0f,1.0f };
	}
	//-- メンバ変数 --
	int m_nSlice;
	int m_nStack;
	float m_fRadius;
	DirectX::XMFLOAT4 m_Dif;
};
class PrimitiveInfoBoxField
{
	friend class Primitive;
public:
	PrimitiveInfoBoxField()
	{
		m_Size = { 1.0f,1.0f,1.0f };
		m_Dif = { 1.0f,1.0f,1.0f,1.0f };
		m_pTex = nullptr;
	}
	//-- メンバ変数 --
	DirectX::XMFLOAT3 m_Size;
	DirectX::XMFLOAT4 m_Dif;
	ID3D11ShaderResourceView* m_pTex;

}; 

class Primitive
{
public:
	static Mesh* CreateLinePrimitive(Object*, PrimitiveInfoLine);
	static Mesh* CreateWireCubePrimitive(Object*, PrimitiveInfoWireCube);
	static Mesh* CreateSpherePrimitive(Object*, PrimitiveInfoSphere);
	static Mesh* CreateCubePrimitive(Object*, PrimitiveInfoCube);
	static Mesh* CreateFieldPrimitive(Object*, PrimitiveInfoField);
	static Mesh* CreateBoxFieldPrimitive(Object*, PrimitiveInfoBoxField);
	static Mesh* Create2DObjectPrimitive(Object*, PrimitiveInfo2DObj);
	static Mesh* CreateSlopePrimitive(Object*, PrimitiveInfoSlope);
};
