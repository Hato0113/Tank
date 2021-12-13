//------------------
// マウス情報
//-- author --
//	HatoriMasashi
//------------------

//-- include --
#include "MouseInfo.h"
using namespace DirectX;
#include "Component\Camera\Camera.h"
#include "IMGUI\GUI_Message.h"
#include "MySystem\KeyInput\KeyInput.h"
#include "Scene\SceneManager.h"

XMFLOAT2 MouseInfo::m_MousePos = { 0.0f,0.0f };


MouseInfo::MouseInfo()
{

}

void MouseInfo::Update()
{
	auto pos = KeyInput::GetMousePos();
	m_MousePos.x = static_cast<float>(pos.x);
	m_MousePos.y = static_cast<float>(pos.y);
	//auto camObj = parent->m_pScene->manager->FindObjectWithName("MainCamera");
	//auto cam = camObj->GetComponent<Camera>();
	//XMFLOAT4X4 viewMat = cam->GetViewMat();
	//XMFLOAT4X4 projectionMat = cam->GetProjMat();
	//
	//float width = WindowInfo::m_ScreenWidth / 2.0f;
	//float height = WindowInfo::m_ScreenHeight / 2.0f;
	//XMMATRIX viewport = {
	//	width,0,0,0,
	//	0,-height,0,0,
	//	0,0,1,0,
	//	width,height,0,1
	//};
	//
	//XMVECTOR currentPos = XMLoadFloat3(&m_Position);
	//currentPos = XMVector3Transform(currentPos, XMLoadFloat4x4(&viewMat));
	//currentPos = XMVector3Transform(currentPos, XMLoadFloat4x4(&projectionMat));
	//
	//XMStoreFloat3(&m_Position, currentPos);
	//IG::MessageManager::DrawLog("Before : %.10f", m_Position.z);
	//m_Position.z += cam->GetNear();
	//m_Position.z *= cam->GetFar() / (cam->GetFar() + cam->GetNear());
	//IG::MessageManager::DrawLog("After : %.10f", m_Position.z);
	//XMVECTOR viewVec = XMVectorSet(m_Position.x / m_Position.z, m_Position.y / m_Position.z, 1.0f, 1.0f);
	//XMVECTOR temp = XMVector3Transform(viewVec, viewport);
	//XMStoreFloat2(&m_ScreenPos, temp);
	//m_MousePos = m_ScreenPos;

	IG::MessageManager::DrawLog("mousePos = %.3f / %.3f", m_MousePos.x, m_MousePos.y);
	auto vec = GetMouseVec();
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, vec);
	IG::MessageManager::DrawLog("mouseVec = %.10f / %.10f / %.10f", temp.x, temp.y, temp.z);
}

XMVECTOR MouseInfo::GetMouseVec()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	if (scene)
	{
		auto camObj = scene->manager->FindObjectWithName("MainCamera");
		if (!camObj)
		{
			XMFLOAT3 temp(0.0f, 0.0f, 0.0f);
			return XMLoadFloat3(&temp);
		}
		auto cam = camObj->GetComponent<Camera>();

		//-- スクリーン座標 -> ワールド座標 --
		float width = WindowInfo::m_ScreenWidth / 2.0f;
		float height = WindowInfo::m_ScreenHeight / 2.0f;
		XMMATRIX viewport = {
			width,0,0,0,
			0,-height,0,0,
			0,0,1,0,
			width,height,0,1
		};

		XMFLOAT4X4 viewMat = cam->GetViewMat();
		XMFLOAT4X4 projectionMat = cam->GetProjMat();

		XMMATRIX invViewMat = XMMatrixInverse(nullptr, (XMLoadFloat4x4(&viewMat)));
		XMMATRIX invProjectionMat = XMMatrixInverse(nullptr, (XMLoadFloat4x4(&projectionMat)));
		XMMATRIX invViewPortMat = XMMatrixInverse(nullptr, viewport);
		XMMATRIX temp = invViewMat * invProjectionMat * invViewPortMat;
		XMVECTOR NearVec;
		XMVECTOR FarVec;
		XMVECTOR viewVec = XMVectorSet(m_MousePos.x, m_MousePos.y, 0.0f, 1.0f);
		NearVec = XMVector3TransformCoord(viewVec, temp);
		viewVec = XMVectorSet(m_MousePos.x, m_MousePos.y, 1.0f, 1.0f);
		FarVec = XMVector3TransformCoord(viewVec, temp);
		XMVECTOR ray;
		ray = FarVec - NearVec;
		ray = XMVector3Normalize(ray);

		return ray;
	}
	else
	{
		XMFLOAT3 temp(0.0f, 0.0f, 0.0f);
		return XMLoadFloat3(&temp);
	}
}


