//--------------------------
//	Effekseerエフェクト管理
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "EffectManager.h"
#include "MySystem\DirectX.h"
#include "Component\Effect\Effect.h"
#include "Scene\SceneManager.h"
#include "Component\Camera\Camera.h"
#include "MySystem\main.h"

::EffekseerRendererDX11::RendererRef EffectManager::m_Renderer;
::Effekseer::ManagerRef EffectManager::m_Manager;
std::unordered_map<EffectID, ::Effekseer::EffectRef> EffectManager::m_EffectMap;

void EffectManager::Init()
{
	auto dc = DX::DirectXManager::GetInstance().GetDeviceContext();
	auto device = DX::DirectXManager::GetInstance().GetDevice();

	m_Renderer = ::EffekseerRendererDX11::Renderer::Create(device, dc, 8000);
	m_Manager = ::Effekseer::Manager::Create(8000);

	//-- 描画モジュール設定 --
	m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
	m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
	m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
	m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
	m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

	//-- ローダー設定 --
	m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
	m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
	m_Manager->SetMaterialLoader(m_Renderer->CreateMaterialLoader());
	m_Manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

void EffectManager::Uninit()
{
	//-- 破棄 --
	m_Manager.Reset();
	m_Renderer.Reset();
}

void EffectManager::Update()
{
	m_Manager->Update();
}

void EffectManager::Draw()
{
	//-- カメラ行列設定 --
	auto camera = SceneManager::GetInstance().GetCurrentScene()->manager->FindObjectWithName("MainCamera");
	if (!camera) return;	//カメラ設定がされていない
	auto Cam = camera->GetComponent<Camera>();
	auto xmPos = camera->transform->GetPos();
	auto effPos = ::Effekseer::Vector3D(xmPos.x, xmPos.y, xmPos.z);
	auto xmTarget = Cam->GetTarget();
	auto effTarget = ::Effekseer::Vector3D(xmTarget.x, xmTarget.y, xmTarget.z);


	m_Renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		Cam->GetFov() / 180.0f * 3.14f, (float)WindowInfo::m_ScreenWidth / (float)WindowInfo::m_ScreenHeight, Cam->GetNClip(), Cam->GetFClip()));

	m_Renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(effPos, effTarget, ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	m_Renderer->BeginRendering();

	m_Manager->Draw();

	m_Renderer->EndRendering();
}

::Effekseer::EffectRef EffectManager::Get(EffectID id)
{
	return m_EffectMap[id];
}

void EffectManager::Load(EffectID id, std::u16string fileName)
{
	auto effect = Effekseer::Effect::Create(m_Manager, fileName.c_str());
	m_EffectMap.insert(std::make_pair(id, effect));
}
