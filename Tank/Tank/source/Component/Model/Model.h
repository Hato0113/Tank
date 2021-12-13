﻿//------------------------------
//	モデルコンポーネント
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "MySystem\Model\ModelManager.h"

//-- クラス定義 --
class Model : public Component
{
public:
	Model();
	virtual ~Model();

	void Update() override;
	void Draw() override;

	//-- セット関数 --
public:
	void SetModel(CAssimpModel* pmodel) { m_pModel = pmodel; }
	void SetScale(DirectX::XMFLOAT3 scale);
	void SetScale(float scale);
	void SetRelativePos(DirectX::XMFLOAT3 relPos) { m_RelativePos = relPos; }
	void SetVertex(std::string name) { m_VertexShaderName = name; }
	void SetPixel(std::string name) { m_PixelShaderName = name; }
	void SetUseLight(bool state) { m_useLightFlag = state; }

	//-- ゲット関数 --
public:
	CAssimpModel* GetModel() { return m_pModel; }

	//-- メンバ変数 --
private:
	CAssimpModel* m_pModel;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT3 m_RelativePos;	//相対座標
	bool IsCameraLightSet = false;
	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
	bool m_useLightFlag;
	DirectionalLight* m_pLight;
};