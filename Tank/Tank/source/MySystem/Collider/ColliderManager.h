//-------------------------
//	コライダーマネージャー
//-- author --
//	HatoriMasashi
//-------------------------
#pragma once

//-- include --
#include <vector>
#include "Component\Collider\Collider.h"

//-- クラス定義 --
class ColliderManager final
{
private:
	ColliderManager() = default;
	~ColliderManager() = default;
public:
	void Init();
	void Update();
	void Uninit();

	void Reset();
	void Add(Collider*);
	void Remove(Collider*);

	static ColliderManager& GetInstance()
	{
		static ColliderManager inst;
		return inst;
	}
public:
	std::vector<Collider*> m_ColliderArray;
};

