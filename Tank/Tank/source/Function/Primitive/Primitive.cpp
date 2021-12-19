//----------------------------------
//	プリミティブ形状
//-- author --
//	HatoriMasashi
//----------------------------------

//-- include --
#include "Primitive.h"
#include "MySystem\Mesh\MeshManager.h"
using namespace DirectX;

/*
	線メッシュ生成
	引数	親オブジェクトポインタ
			線情報
*/
Mesh* Primitive::CreateLinePrimitive(Object* parent, PrimitiveInfoLine info)
{
	//-- メッシュコンポーネント生成 --
	Mesh* pMesh = parent->AddComponent<Mesh>();
	pMesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pMesh->SetNumIndex(info.IndexNum);
	pMesh->SetNumVertex(info.IndexNum);
	pMesh->SetLightUse(false);

	//-- 作業用領域確保 --
	int* pIndex = new int[info.IndexNum];
	MeshVertexInfo* pVertex = new MeshVertexInfo[info.IndexNum];

	//-- 頂点情報作成 --
	pVertex[0].Vertex = info.m_startPos;
	pVertex[1].Vertex = info.m_endPos;

	pVertex[0].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertex[1].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	pVertex[0].color = info.m_startDif;
	pVertex[1].color = info.m_endDif;

	pVertex[0].uv = XMFLOAT2(0.0f, 0.0f);
	pVertex[1].uv = XMFLOAT2(1.0f, 1.0f);

	//-- インデックス情報作成 --
	pIndex[0] = 0;
	pIndex[1] = 1;

	//-- メッシュ情報生成 --
	MeshManager::MakeMeshInfo(pMesh, pVertex, pIndex);

	delete[] pIndex;
	delete[] pVertex;

	return pMesh;
}

/*
	ワイヤーキューブメッシュ描画
	引数	親オブジェクトポインタ
			キューブ情報
*/
Mesh* Primitive::CreateWireCubePrimitive(Object* parent, PrimitiveInfoWireCube info)
{
	//-- メッシュコンポーネント生成 --
	Mesh* pMesh = parent->AddComponent<Mesh>();
	pMesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pMesh->SetNumIndex(info.IndexNum);
	pMesh->SetNumVertex(info.VertexNum);
	pMesh->SetLightUse(false);

	//-- 作業用領域確保 --
	int* pIndex = new int[info.IndexNum];
	MeshVertexInfo* pVertex = new MeshVertexInfo[info.VertexNum];

	static XMFLOAT3 Vertexs[8];
	Vertexs[0] = XMFLOAT3(-1.0f * info.m_Size.x, 1.0f * info.m_Size.y, -1.0f * info.m_Size.z);		//上左手前
	Vertexs[1] = XMFLOAT3(-1.0f * info.m_Size.x, 1.0f * info.m_Size.y, 1.0f * info.m_Size.z);		//上左奥
	Vertexs[2] = XMFLOAT3(1.0f * info.m_Size.x, 1.0f * info.m_Size.y, -1.0f * info.m_Size.z);		//上右手前
	Vertexs[3] = XMFLOAT3(1.0f * info.m_Size.x, 1.0f * info.m_Size.y, 1.0f * info.m_Size.z);		//上右奥
	Vertexs[4] = XMFLOAT3(-1.0f * info.m_Size.x, -1.0f * info.m_Size.y, -1.0f * info.m_Size.z);		//下左手前
	Vertexs[5] = XMFLOAT3(-1.0f * info.m_Size.x, -1.0f * info.m_Size.y, 1.0f * info.m_Size.z);		//下左奥
	Vertexs[6] = XMFLOAT3(1.0f * info.m_Size.x, -1.0f * info.m_Size.y, -1.0f * info.m_Size.z);		//下右手前
	Vertexs[7] = XMFLOAT3(1.0f * info.m_Size.x, -1.0f * info.m_Size.y, 1.0f * info.m_Size.z);		//下右奥

	for (int i = 0; i < info.VertexNum; i++)
	{
		pVertex[i].Vertex = Vertexs[i];
		pVertex[i].color = info.m_Dif;
		pVertex[i].normal = { 0.0f,0.0f,0.0f };
		pVertex[i].uv = { 0.0f,(float)i / info.VertexNum };
	}

	struct CubeIndex
	{
		int s, e;
	};
	CubeIndex* pCI = new CubeIndex[info.IndexNum / 2];

	pCI[0] = { 0,1 };
	pCI[1] = { 1,3 };
	pCI[2] = { 3,2 };
	pCI[3] = { 2,0 };
	pCI[4] = { 0,4 };
	pCI[5] = { 1,5 };
	pCI[6] = { 2,6 };
	pCI[7] = { 3,7 };
	pCI[8] = { 4,5 };
	pCI[9] = { 5,7 };
	pCI[10] = { 7,6 };
	pCI[11] = { 6,4 };


	//-- インデックス情報作成 --
	for (int i = 0; i < info.IndexNum / 2; i++)
	{
		pIndex[i * 2] = pCI[i].s;
		pIndex[i * 2 + 1] = pCI[i].e;
	}


	//-- メッシュ情報生成 --
	MeshManager::MakeMeshInfo(pMesh, pVertex, pIndex);

	delete[] pIndex;
	delete[] pVertex;

	return pMesh;
}

/*
	キューブメッシュ描画
	引数	親オブジェクトポインタ
			キューブ情報
*/
Mesh* Primitive::CreateCubePrimitive(Object* parent, PrimitiveInfoCube info)
{
	Mesh* pMesh = parent->AddComponent<Mesh>();

	//-- サイズ定義 --
	XMFLOAT3 vSize;
	vSize.x = info.m_Size.x * 1.0f;
	vSize.y = info.m_Size.y * 1.0f;
	vSize.z = info.m_Size.z * 1.0f;
	//-- インデックス / 定数定義 --
	const float Vertex[] = {
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
	};
	const float normal[] = {
		 0.0f,  0.0f, -1.0f,
		 1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
	};
	const int face[] = {
		4, 0, 5, 1, 5, 0,
		5, 1, 7, 3, 7, 1,
		7, 3, 6, 2, 6, 3,
		6, 2, 4, 0, 4, 2,
		0, 2, 1, 3, 1, 2,
		6, 4, 7, 5, 7, 4,
	};
	const float uv[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};
	//-- 頂点情報 / インデックス配列生成 --
	pMesh->SetNumIndex(info.IndexNum);
	pMesh->SetNumVertex(info.VertexNum);
	MeshVertexInfo* pVertexWk = new MeshVertexInfo[info.VertexNum];
	int* pIndexWk = new int[info.IndexNum];
	MeshVertexInfo* pVtx = pVertexWk;
	int nIdx = 0;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			pVtx->Vertex.x = Vertex[face[i * 6 + j] * 3 + 0] * info.m_Size.x;
			pVtx->Vertex.y = Vertex[face[i * 6 + j] * 3 + 1] * info.m_Size.y;
			pVtx->Vertex.z = Vertex[face[i * 6 + j] * 3 + 2] * info.m_Size.z;
			pVtx->normal.x = normal[i * 3 + 0];
			pVtx->normal.y = normal[i * 3 + 1];
			pVtx->normal.z = normal[i * 3 + 2];
			pVtx->uv.x = uv[j * 2 + 0];
			pVtx->uv.y = uv[j * 2 + 1];
			pVtx->color = XMFLOAT4(1, 1, 1, 1);
			++pVtx;
			pIndexWk[i * 6 + j] = nIdx;
			++nIdx;
		}
		pIndexWk[i * 6 + 4] = pIndexWk[i * 6 + 2];
		pIndexWk[i * 6 + 5] = pIndexWk[i * 6 + 1];
	}
	//-- マテリアル設定 --
	MATERIAL mat;
	mat.Reset();
	mat.Diffuse = info.m_Dif;
	pMesh->SetMaterial(mat);
	MeshManager::MakeMeshInfo(pMesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;
	return pMesh;
}

/*
	スロープメッシュ
*/
Mesh* Primitive::CreateSlopePrimitive(Object* parent, PrimitiveInfoSlope info)
{
	Mesh* pMesh = parent->AddComponent<Mesh>();
	pMesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const int VertexNum = 18;
	const int IndexNum = 24;
	pMesh->SetNumVertex(VertexNum);
	pMesh->SetNumIndex(IndexNum);
	MeshVertexInfo* pVertex = new MeshVertexInfo[VertexNum];
	int* pIndex = new int[IndexNum];
	DirectX::XMFLOAT3 Vertexs[6] =
	{
		{-info.m_Size.x,info.m_Size.y,info.m_Size.z},		//上左奥
		{info.m_Size.x,info.m_Size.y,info.m_Size.z},		//上右奥
		{-info.m_Size.x,-info.m_Size.y,info.m_Size.z},		//下左奥
		{info.m_Size.x,-info.m_Size.y,info.m_Size.z},		//下右奥
		{-info.m_Size.x,-info.m_Size.y,-info.m_Size.z},		//下左手前
		{info.m_Size.x,-info.m_Size.y,-info.m_Size.z},		//下右手前
	};
	DirectX::XMFLOAT3 Diagonal = { 0.0f,info.m_Size.z,info.m_Size.y };
	DirectX::XMFLOAT3 Normals[5] =
	{
		{Diagonal}, //斜め面
		{0.0f,0.0f,1.0f},	//背面
		{-1.0f,0.0f,0.0f},//左面
		{1.0f,0.0f,0.0f},//右面
		{0.0f,-1.0f,0.0f},//底面
	};
	DirectX::XMFLOAT2 uv = { 0.0f,0.0f };

	for (int i = 0; i < VertexNum; i++)
	{
		pVertex[i].uv = uv;
		pVertex[i].color = { 1,1,1,1 };
	}
	const int v[VertexNum] =
	{
		0,1,4,5, 0,1,2,3, 5,1,3, 4,2,0, 2,3,4,5
	};
	const int n[VertexNum] =
	{
		0,0,0,0, 1,1,1,1, 3,3,3, 2,2,2, 4,4,4,4
	};
	for (int i = 0; i < VertexNum; i++)
	{
		pVertex[i].Vertex = Vertexs[v[i]];
		pVertex[i].normal = Normals[n[i]];
	}
	const int index[IndexNum] =
	{
		0,1,2,2,1,3, 5,4,7,7,4,6, 8,9,10, 11,12,13, 15,14,17,17,14,16,
	};
	for (int i = 0; i < IndexNum; i++)
	{
		pIndex[i] = index[i];
	}

	MATERIAL mat;
	mat.Reset();
	mat.Diffuse = info.m_Dif;
	pMesh->SetMaterial(mat);
	MeshManager::MakeMeshInfo(pMesh, pVertex, pIndex);
	delete[] pIndex;
	delete[] pVertex;
	return pMesh;
}

/*
	フィールドメッシュ情報
	引数	親オブジェクトポインタ
			フィールド情報
*/
Mesh* Primitive::CreateFieldPrimitive(Object* parent, PrimitiveInfoField info)
{
	Mesh* pMesh = parent->AddComponent<Mesh>();
	pMesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pMesh->SetTex(info.m_pTex);

	pMesh->SetNumIndex(info.m_IndexNum);
	pMesh->SetNumVertex(info.m_VertexNum);
	MeshVertexInfo* pVertex = new MeshVertexInfo[info.m_VertexNum];
	int* pIndex = new int[info.m_IndexNum];

	//-- 頂点情報設定 --
	pVertex[0].Vertex = XMFLOAT3(-info.m_Size.x, 0.0f, info.m_Size.y);
	pVertex[1].Vertex = XMFLOAT3(info.m_Size.x, 0.0f, info.m_Size.y);
	pVertex[2].Vertex = XMFLOAT3(-info.m_Size.x, 0.0f, -info.m_Size.y);
	pVertex[3].Vertex = XMFLOAT3(info.m_Size.x, 0.0f, -info.m_Size.y);

	pVertex[0].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertex[1].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertex[2].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pVertex[3].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	pVertex[0].color = info.m_Color;
	pVertex[1].color = info.m_Color;
	pVertex[2].color = info.m_Color;
	pVertex[3].color = info.m_Color;

	float u = static_cast<float>(info.m_Split.x);
	float v = static_cast<float>(info.m_Split.y);
	pVertex[0].uv = XMFLOAT2(0.0f, 0.0f);
	pVertex[1].uv = XMFLOAT2(u, 0.0f);
	pVertex[2].uv = XMFLOAT2(0.0f, v);
	pVertex[3].uv = XMFLOAT2(u, v);

	//-- インデックス情報設定 --
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 3;

	MeshManager::MakeMeshInfo(pMesh, pVertex, pIndex);
	delete[] pIndex;
	delete[] pVertex;
	return pMesh;

}

/*
	2Dオブジェクト
*/
Mesh* Primitive::Create2DObjectPrimitive(Object* parent, PrimitiveInfo2DObj info)
{
	Mesh* pMesh = parent->AddComponent<Mesh>();
	pMesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pMesh->SetTex(info.m_pTex);

	const int IndexNum = 4;
	const int VertexNum = 4;
	pMesh->SetNumIndex(IndexNum);
	pMesh->SetNumVertex(VertexNum);
	MeshVertexInfo* pVertex = new MeshVertexInfo[VertexNum];
	int* pIndex = new int[IndexNum];

	//-- 頂点情報設定 --
	pVertex[0].Vertex = XMFLOAT3(-info.m_Size.x, info.m_Size.y, 0.0f);
	pVertex[1].Vertex = XMFLOAT3(info.m_Size.x, info.m_Size.y, 0.0f);
	pVertex[2].Vertex = XMFLOAT3(-info.m_Size.x, -info.m_Size.y, 0.0f);
	pVertex[3].Vertex = XMFLOAT3(info.m_Size.x, -info.m_Size.y, 0.0f);

	pVertex[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);	//手前に法線を向ける
	pVertex[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertex[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertex[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	pVertex[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	pVertex[0].uv = XMFLOAT2(info.m_UV.x, info.m_UV.y);
	pVertex[1].uv = XMFLOAT2(info.m_UV.x + info.m_FrameSize.x, info.m_UV.y);
	pVertex[2].uv = XMFLOAT2(info.m_UV.x, info.m_UV.y + info.m_FrameSize.y);
	pVertex[3].uv = XMFLOAT2(info.m_UV.x + info.m_FrameSize.x, info.m_UV.y + info.m_FrameSize.y);

	//-- インデックス情報設定 --
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 3;

	MeshManager::MakeMeshInfo(pMesh, pVertex, pIndex);
	delete[] pIndex;
	delete[] pVertex;
	return pMesh;
}


/*
	球メッシュ情報
	引数	親オブジェクトポインタ
			球情報
*/
Mesh* Primitive::CreateSpherePrimitive(Object* parent, PrimitiveInfoSphere info)
{
	Mesh* pMesh = parent->AddComponent<Mesh>();
	pMesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	int nVertex = (info.m_nSlice + 1) * (info.m_nStack + 1);
	int nFace = info.m_nSlice * info.m_nStack * 2;
	pMesh->SetNumIndex(nFace * 3);
	pMesh->SetNumVertex(nVertex);

	MeshVertexInfo* pVertexWk = new MeshVertexInfo[nVertex];
	int* pIndexWk = new int[nFace * 3];
	// 頂点、法線ベクトル、テクスチャ座標の位置
	MeshVertexInfo* pVtx = pVertexWk;
	for (int j = 0; j <= info.m_nStack; ++j) {
		float v = (float)j / (float)info.m_nStack;
		float ph = XM_PI * v;
		float y = cosf(ph);
		float r = sinf(ph);
		for (int i = 0; i <= info.m_nSlice; ++i) {
			float u = (float)i / (float)info.m_nSlice;
			float th = XM_2PI * u;
			float x = r * cosf(th);
			float z = r * sinf(th);
			pVtx->Vertex.x = x * info.m_fRadius;
			pVtx->Vertex.y = y * info.m_fRadius;
			pVtx->Vertex.z = z * info.m_fRadius;
			pVtx->normal.x = x;
			pVtx->normal.y = y;
			pVtx->normal.z = z;
			pVtx->uv.x = u;
			pVtx->uv.y = v;
			pVtx->color = info.m_Dif;
			++pVtx;
		}
	}
	// 面のインデックス
	int* pIdx = pIndexWk;
	for (int j = 0; j < info.m_nStack; ++j) {
		for (int i = 0; i < info.m_nSlice; ++i) {
			int nIndex = (info.m_nSlice + 1) * j + i;
			// 左下半分
			*pIdx++ = nIndex + (info.m_nSlice + 1);
			*pIdx++ = nIndex;
			*pIdx++ = nIndex + (info.m_nSlice + 1) + 1;
			// 右上半分
			*pIdx++ = nIndex + 1;
			*pIdx++ = nIndex + (info.m_nSlice + 1) + 1;
			*pIdx++ = nIndex;
		}
	}
	MeshManager::MakeMeshInfo(pMesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;
	return pMesh;
}

/*
	ボックス型フィールド
	テクスチャ付き
*/
Mesh* Primitive::CreateBoxFieldPrimitive(Object* parent, PrimitiveInfoBoxField info)
{
	Mesh* pMesh = parent->AddComponent<Mesh>();

	//-- サイズ定義 --
	XMFLOAT3 vSize;
	vSize.x = info.m_Size.x * 1.0f;
	vSize.y = info.m_Size.y * 1.0f;
	vSize.z = info.m_Size.z * 1.0f;
	//-- インデックス / 定数定義 --
	const static float Vertex[] = {
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
	};
	const static float normal[] = {
		 0.0f,  0.0f, -1.0f,
		 1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
	};
	const static int face[] = {
		4, 0, 5, 1, 5, 0,
		5, 1, 7, 3, 7, 1,
		7, 3, 6, 2, 6, 3,
		6, 2, 4, 0, 4, 2,
		0, 2, 1, 3, 1, 2,
		6, 4, 7, 5, 7, 4,
	};
	const static float uv[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};
	//-- 頂点情報 / インデックス配列生成 --
	const int indexNum = 36;
	const int vertexNum = 24;
	pMesh->SetNumIndex(indexNum);
	pMesh->SetNumVertex(vertexNum);
	MeshVertexInfo* pVertexWk = new MeshVertexInfo[vertexNum];
	int* pIndexWk = new int[indexNum];
	MeshVertexInfo* pVtx = pVertexWk;
	int nIdx = 0;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			pVtx->Vertex.x = Vertex[face[i * 6 + j] * 3 + 0] * info.m_Size.x;
			pVtx->Vertex.y = Vertex[face[i * 6 + j] * 3 + 1] * info.m_Size.y;
			pVtx->Vertex.z = Vertex[face[i * 6 + j] * 3 + 2] * info.m_Size.z;
			pVtx->normal.x = normal[i * 3 + 0];
			pVtx->normal.y = normal[i * 3 + 1];
			pVtx->normal.z = normal[i * 3 + 2];
			pVtx->uv.x = uv[j * 2 + 0];
			pVtx->uv.y = uv[j * 2 + 1];
			pVtx->color = XMFLOAT4(1, 1, 1, 1);
			++pVtx;
			pIndexWk[i * 6 + j] = nIdx;
			++nIdx;
		}
		pIndexWk[i * 6 + 4] = pIndexWk[i * 6 + 2];
		pIndexWk[i * 6 + 5] = pIndexWk[i * 6 + 1];
	}
	//-- マテリアル設定 --
	MATERIAL mat;
	mat.Reset();
	mat.Diffuse = info.m_Dif;
	pMesh->SetMaterial(mat);
	pMesh->SetTex(info.m_pTex);
	MeshManager::MakeMeshInfo(pMesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;
	return pMesh;
}
