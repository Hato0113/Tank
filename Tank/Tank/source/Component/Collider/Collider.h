//--------------------------
//	コライダーベースクラス
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include <vector>
#include "Objects\Object.h"
#include "Component\Mesh\Mesh.h"

//-- enum --
enum class ColliderType	//ダウンキャストする際に必要な情報
{
	Box,
	Sphere,
};

//-- クラス定義 --
class ColliderManager;
class Collider abstract : public Component
{
	friend class ColliderManager;
public:
	Collider();
	virtual ~Collider();

	void Hit();

	void SetSubjectState(bool state) { m_IsSubject = state; }
	ColliderType GetType() { return m_Type; }

private:
	static bool AABB(Collider* my, Collider* other);	//Box同士
	static bool Sphere(Collider* my, Collider* other);	//どちらかに球が含まれている

	//-- メンバ変数 --
private:
	bool m_IsSubject;		//自身が主体となるか
	std::vector<Collider*> m_PrevHitArray;
protected:
	ColliderType m_Type;	//自身のコライダータイプ
public:
	Mesh* m_pMesh;		//判定可視化用
};

/*
	自身が主体とならなければUpdateで判定を取らない
	コライダータイプを合わせるためキャストに時間がかかる
*/


