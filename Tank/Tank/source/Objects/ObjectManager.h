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
	void Update();
	void Draw();

	//-- オブジェクト操作 --
	void Add(Object*);
	std::vector<Object*> FindObjectsWithName(std::string name);
	std::vector<Object*> FindObjectsWithTag(std::string tag);
	Object* FindObjectWithName(std::string);
	Object* FindObjectWithTag(std::string);

	std::vector<Object*> GetList() { return m_ObjectList; }

	//-- メンバ変数 --
public:
	SceneBase* m_pScene;
private:
	std::vector<Object*> m_ObjectList;
};


