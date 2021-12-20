//--------------------------
//	アプリケーション管理
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "Application.hpp"
using namespace tank;
#include "Allocator\Allocator.hpp"
#include "Scene\SceneManager.h"
#include "Scene\SceneGame.h"
#include "Scene\SceneTitle.h"
#include "Scene\ConstantScene.h"
#include "Scene\SceneLoad.h"
#include "MySystem\Mesh\MeshManager.h"
#include "MySystem\Polygon\PolygonManager.h"
#include "MySystem\Texture\TextureManager.h"
#include "System\AssimpModel.h"
#include "MySystem\Model\ModelManager.h"
#include "MySystem\Sprite\SpriteManager.h"
#include "IMGUI\MyImGui.h"
#include "FontManager\FontManager.h"
#include "Shader\ShaderManager.h"
#include "Effect\EffectManager.h"
#include "Resident\ResidentData.h"
#include "Scene\SceneLevelEditor.h"

//-- 静的メンバ --
bool Application::m_GameFlag = true;

namespace
{
	void* mainMemory{ nullptr };
	size_t memorySize{ 1 * 1024 * 1024 };
}

void Application::Init()
{
	//-- IMGUI初期化 --
	IG::IMGUIManager::GetInstance().Init();

	//-- アロケータ初期化 --
	mainMemory = new char[memorySize];
	Allocator::Init(mainMemory, memorySize);

	//-- シェーダー初期化 --
	MeshManager::Init();
	PolygonManager::Init();
	CAssimpModel::InitShader(DX::DirectXManager::GetInstance().GetDevice());
	ShaderManager::GetInstance().Init();

	//-- リソース管理初期化 --
	TextureManager::Init();
	ModelManager::Init();
	SpriteManager::Init();
	FontManager::Init();
	EffectManager::Init();
	ResidentDataManager::Init();

	//-- シーン初期化 --
	SceneManager::GetInstance().Add(SceneType::Title, Allocator::Alloc<SceneTitle>());
	SceneManager::GetInstance().Add(SceneType::Game, Allocator::Alloc<SceneGame>());
	SceneManager::GetInstance().Add(SceneType::Load, Allocator::Alloc<SceneLoad>());
	SceneManager::GetInstance().Add(SceneType::Edit, Allocator::Alloc<SceneLevelEditor>());
	ConstantScene::GetInstance().Init();
	SceneManager::GetInstance().SetFirst(SceneType::Load);
}

void Application::Update()
{
	ConstantScene::GetInstance().Update();
	SceneManager::GetInstance().Update();

	//-- IMGUI更新 --
	IG::IMGUIManager::GetInstance().Update();
}

void Application::Draw()
{
	SceneManager::GetInstance().Draw();
	ConstantScene::GetInstance().Draw();	//常駐の情報は前面に出す。

	//-- IMGUI描画 --
	IG::IMGUIManager::GetInstance().Draw();
}

void Application::Uninit()
{
	//-- シーン終了 --
	SceneManager::GetInstance().Uninit();
	ConstantScene::GetInstance().Uninit();

	//-- シェーダー終了 --
	MeshManager::Uninit();
	PolygonManager::Uninit();
	CAssimpModel::UninitShader();
	ShaderManager::GetInstance().Uninit();

	//-- リソース管理終了 --
	TextureManager::Uninit();
	ModelManager::Uninit();
	SpriteManager::Uninit();
	FontManager::Uninit();
	ResidentDataManager::Uninit();

	//-- アロケータ終了 --
	Allocator::Uninit();
	delete[] mainMemory;

	//-- IMGUI終了処理 --
	IG::IMGUIManager::GetInstance().Uninit();
}