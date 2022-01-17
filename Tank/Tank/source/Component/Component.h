//--------------------------
//	コンポーネントベースクラス
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- enum --
enum class Layer
{
	Default = 0,	//所属レイヤー無し(描画しない)
	Back3D,
	Front3D,
	Back2D,
	Front2D,

	LayerMax
};

//-- クラス定義 --
class Object;
class Collider;
class Component abstract
{
public:
	Component();
	virtual ~Component() {}

	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Uninit() {};

	
	virtual void OnCollisionEnter(Collider*) {};	//当たった瞬間
	virtual void OnCollisionStay(Collider*) {};	//当たっている間
	virtual void OnCollisionExit(Collider*) {};	//当たらなくなった瞬間

	//-- アクセサ --
	void SetLayer(const Layer layer) { m_Layer = layer; }
	void SetState(const bool state) { m_State = state; }
	void SetActive(const bool state) { m_Active = state; }

	Layer GetLayer() const { return m_Layer; }
	bool GetState() const { return m_State; }
	bool GetActive() const { return m_Active; }

	//-- メンバ変数 --
public:
	Object* parent;	//親オブジェクト
private:
	Layer m_Layer;		//表示レイヤー
	bool m_State;		//コンポーネント状態 , falseでコンポーネント削除
	bool m_Active;		//アクティブ状態
};

#define COMPONENT_H
//-- include --
#include "Objects\Object.h"
