//--------------------------
//	オブジェクト管理
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Scene/SceneBase.h"
#include "Object.h"
#include <vector>

//-- クラス定義 --
enum class ObjectTag;
class Object;
class ObjectManager	final
{
public:
	ObjectManager();
	~ObjectManager();

public:
	/*
		更新関数
		パラメータ無し
	*/
	void Update();
	/*
		描画関数
		パラメータ無し
	*/
	void Draw();

	//-- オブジェクト操作 --
	/*
		オブジェクト追加
		引数 : オブジェクトへのポインタ
	*/
	void Add(Object*);
	/*
		オブジェクト検索<name>
		引数 : 検索したい名前
		戻り値 : オブジェクトへのポインタのvector
	*/
	std::vector<Object*> FindObjectsWithName(std::string name);
	/*
		オブジェクト検索<tag>
		引数 : 検索したいタグ
		戻り値 : オブジェクトへのポインタのvector
	*/
	std::vector<Object*> FindObjectsWithTag(std::string tag);
	/*
		オブジェクト検索<name>
		引数 : 検索したい名前
		戻り値 : オブジェクトへのポインタ
	*/
	Object* FindObjectWithName(std::string);
	/*
		オブジェクト検索<tag>
		引数 : 検索したいタグ
		戻り値 : オブジェクトへのポインタ
	*/
	Object* FindObjectWithTag(std::string);

	/*
		オブジェクトリスト取得
		戻り値 : オブジェクトリスト
	*/
	std::vector<Object*> GetList() { return m_ObjectList; }

	//-- メンバ変数 --
public:
	SceneBase* m_pScene;	//所属シーンポインタ
private:
	std::vector<Object*> m_ObjectList;
};


