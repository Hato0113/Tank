//-----------------------------
//	メッシュコンポーネント
//-- author --
//	HatoriMasashi
//-----------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "MySystem\Mesh\MeshManager.h"

//-- define --
#define MAT_DIFFUSE		(DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
#define MAT_AMBIENT		(DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
#define MAT_SPECULAR	(DirectX::XMFLOAT4(0.1f,0.1f,0.1f,1.0f))
#define MAT_EMISSIVE	(DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f))


//-- 構造体 --
struct MATERIAL
{
	DirectX::XMFLOAT4	Diffuse;		//拡散反射光
	DirectX::XMFLOAT4	Ambient;		//環境反射
	DirectX::XMFLOAT4	Specular;		//鏡面反射
	DirectX::XMFLOAT4	Emissive;		//放射光
	float		Power;					//Specularの減衰強度
	MATERIAL()
	{
		Reset();
	}
	void Set(MATERIAL mat)
	{
		Diffuse = mat.Diffuse;
		Ambient = mat.Ambient;
		Specular = mat.Specular;
		Emissive = mat.Emissive;
		Power = mat.Power;
	}
	void Reset()
	{
		Diffuse = MAT_DIFFUSE;
		Ambient = MAT_AMBIENT;
		Specular = MAT_SPECULAR;
		Emissive = MAT_EMISSIVE;
		Power = 50.0f;
	}
};

struct MeshVertexInfo		//Mesh_vsに投げる頂点情報
{
	MeshVertexInfo()
	{
		Vertex = { 0.0f,0.0f,0.0f };
		color = { 1.0f,1.0f,1.0f,1.0f };
		uv = { 0.0f,0.0f };
		normal = { 0.0f,0.0f,0.0f };
	}
	DirectX::XMFLOAT3 Vertex;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};


//-- クラス定義 --
class Mesh : public Component
{
public:
	Mesh();
	virtual ~Mesh();
	void Draw() override;
	void Update() override;

public:
	MATERIAL& GetMaterial() { return m_Material; }
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType() { return m_PrimitiveType; }
	int GetNumIndex() { return m_NumIndex; }
	int GetNumVertex() { return m_NumVertex; }
	DirectX::XMFLOAT4X4 GetTexMat() { return m_mtxTexture; }
	float HasTexture() { return (pTexture) ? 1.0f : 0.0f; }
	ID3D11Buffer** GetVertexBufpp() { return &m_pVertexBuffer; }
	ID3D11Buffer* GetIndexBuf() { return m_pIndexBuffer; }
	ID3D11Buffer** GetIndexBufpp() { return &m_pIndexBuffer; }
	ID3D11ShaderResourceView** GetTexpp() { return &pTexture; }
	bool GetLightUse() { return useLight; }

	void SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY type) { m_PrimitiveType = type; }
	void SetNumVertex(int num) { m_NumVertex = num; }
	void SetNumIndex(int num) { m_NumIndex = num; }
	void SetTex(ID3D11ShaderResourceView * tex) { pTexture = tex; }
	void SetMaterial(MATERIAL m) { m_Material = m; }
	void SetLightUse(bool state) { useLight = state; }
	void SetDrawFlag(bool state) { DrawFlag = state; }

	void SetAlphaFadeTime(int time);

private:
	DirectX::XMFLOAT4X4 m_mtxTexture;		// テクスチャ マトリックス
	ID3D11Buffer* m_pVertexBuffer;			// 頂点バッファインターフェースへのポインタ
	ID3D11Buffer* m_pIndexBuffer;			// インデックスバッファインターフェースへのポインタ
	int m_NumVertex;						// 総頂点数	
	int m_NumIndex;							// 総インデックス数
	ID3D11ShaderResourceView* pTexture;		// テクスチャ
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveType;	// プリミティブ型
	MATERIAL m_Material;					// マテリアル

	//-- 各種フラグ --
	bool DrawFlag;
	bool useLight;

	//-- メッシュ操作用 --
	int m_AlphaFadeTime;
	int m_AlphaFadeMax;
};
