//----------------------------------
//	Screen.h
//	スクリーン座標への変換
//-- author --
//	HatoriMasahi
//-- Log --
//	2021/06/21 作成
//----------------------------------

#pragma once

//-- include --
#include "Component\Component.h"
#include "Component\Camera\Camera.h"
#include "Scene\SceneManager.h"

class Screen
{
public:
	static DirectX::XMFLOAT2 GetScreenPos(DirectX::XMFLOAT3 pos)
	{
		using namespace DirectX;

		//-- 現在座標取得 --
		XMFLOAT3 Position = pos;

		auto manager = SceneManager::GetInstance().GetCurrentScene()->manager;
		auto obj = manager->FindObjectWithName("MainCamera");
		auto cam = obj->GetComponent<Camera>();

		XMFLOAT4X4 viewMat = cam->GetViewMat();
		XMFLOAT4X4 projectionMat = cam->GetProjMat();

		float width = WindowInfo::m_ScreenWidth / 2.0f;
		float height = WindowInfo::m_ScreenHeight / 2.0f;
		XMMATRIX viewport = {
			width,0,0,0,
			0,-height,0,0,
			0,0,1,0,
			width,height,0,1
		};

		XMVECTOR currentPos = XMLoadFloat3(&Position);
		currentPos = XMVector3Transform(currentPos, XMLoadFloat4x4(&viewMat));
		currentPos = XMVector3Transform(currentPos, XMLoadFloat4x4(&projectionMat));

		XMStoreFloat3(&Position, currentPos);
		Position.z += cam->GetNear();
		Position.z *= cam->GetFar() / (cam->GetFar() + cam->GetNear());
		XMVECTOR viewVec = XMVectorSet(Position.x / Position.z, Position.y / Position.z, 1.0f, 1.0f);
		XMVECTOR temp = XMVector3Transform(viewVec, viewport);
		XMFLOAT2 ret;
		XMStoreFloat2(&ret, temp);
		return ret;
	}
	
	static DirectX::XMVECTOR GetCursorVector();
};

