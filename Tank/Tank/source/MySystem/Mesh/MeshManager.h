//-----------------------------
//	メッシュマネージャー
//-- author --
//	HatoriMasashi
//-----------------------------
#pragma once

//-- include --
#include "Component\Mesh\Mesh.h"

//-- クラス定義 --
class Mesh;
struct MeshVertexInfo;
class MeshManager
{
public:
	static void Init();
	static void Uninit();

	static void Draw(Mesh*);	//メッシュ描画

	static HRESULT MakeMeshInfo(Mesh* pMesh, MeshVertexInfo vertexWk[], int indexWk[]);

};
