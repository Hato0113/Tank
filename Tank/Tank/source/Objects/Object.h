//--------------------------
//	オブジェクトクラス
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Scene\SceneBase.h"
#ifndef COMPONENT_H
#include "Component\Component.h"
#endif
#include <list>
#include "Allocator/Allocator.hpp"
#include "Component\Transform.h"

//-- クラス定義 --
class Component;
class SceneBase;
class Transform;
class Collider;
class Object final
{
public:
	Object();
	~Object();

	//-- メソッド --
public:
	void Update();
	void Draw(Layer);

	//-- 判定イベント --
	void CollisionEnterEvent(Collider*) const;
	void CollisionStayEvent(Collider*) const;
	void CollisionExitEvent(Collider*) const;

	//-- アクセサ --
	void SetLifeTime(const int time) { m_LifeTime = time; }
	void SetState(const bool state) { m_State = state; }
	void SetPause(const bool pause) { m_Pause = pause; }
	void SetScene(SceneBase* const scene) { m_Scene = scene; }
	void SetVisible(const bool state) { m_Visible = state; }
	void SetActive(const bool state) { m_Visible = state; }

	bool GetState() const { return m_State; }
	bool GetPause() const { return m_Pause; }
	SceneBase* GetScene() const { return m_Scene; }
	bool GetVisible() const { return m_Visible; }
	bool GetActive() const { return m_Active; }

	//-- 静的メソッド --
	static Object* Create(std::string name = "default");	//オブジェクト生成

	//-- コンポーネント関連 --
public:
	template<class T>
	T* GetComponent();

	template<class T, class ...Args>
	T* AddComponent(Args && ...args);

	//-- メンバ変数 --
public:
	Transform* transform;	//トランスフォームだけは常に見れるように
private:
	SceneBase* m_Scene;	//所属シーン
	std::list<Component*> m_ComponentList;
	bool m_State;	//オブジェクト状態 , falseでオブジェクト削除
	bool m_Pause;	//停止フラグ , trueでUpdate停止
	int m_LifeTime;	//生存時間 -1 : 無限
	bool m_Visible;	//見えるか否か
	bool m_Active;	//Updateがかかるか否か
};

/*
	コンポーネント取得
	指定されたクラスのコンポーネントへのポインタを取得
*/
template<class T>
T* Object::GetComponent()
{
	for (auto com : m_ComponentList)
	{
		T* buff = dynamic_cast<T*>(com);
		if (buff != nullptr)
			return buff;
	}
	return nullptr;
}

/*
	コンポーネント追加
	指定されたクラスのコンポーネントを引数付きで追加
*/
template<class T, class ...Args>
T* Object::AddComponent(Args && ...args)
{
	T* buff = Allocator::Alloc<T>(args...);
	buff->parent = this;
	m_ComponentList.push_back(buff);
	buff->Init();
	return buff;
}
