//--------------------------
//	トランスフォームクラス
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "MySystem\DirectX.h"
#include <string>
#include "Function\EulerAngles.h"

//-- クラス定義 --
class Transform : public Component
{
public:
	Transform();
	Transform(const std::string name);
	Transform(const std::string name, const std::string tag);
	virtual ~Transform() = default;

	//-- アクセサ --
	std::string GetName() const { return m_Name; }
	std::string GetTag() const { return m_Tag; }
	DirectX::XMFLOAT3 GetPos() const { return m_Pos; }
	EulerAngles GetEulerAngle() const { return m_EulerAngle; }
	DirectX::XMFLOAT3 GetScale() const { return m_Scale; }

	void SetPos(const DirectX::XMFLOAT3 pos) { m_Pos = pos; }
	void AddPos(const DirectX::XMFLOAT3 add);
	void SetScale(const DirectX::XMFLOAT3 scale) { m_Scale = scale; }
	void SetScale(const float scale) { m_Scale = { scale,scale,scale }; }
	void SetRotate(EulerAngles angle) { m_EulerAngle = angle; }
	void SetName(const std::string name) { m_Name = name; }
	void SetTag(const std::string tag) { m_Tag = tag; }

	//-- メンバ変数 --
private:
	DirectX::XMFLOAT3 m_Pos;	//座標
	EulerAngles m_EulerAngle;	//オイラー角
	DirectX::XMFLOAT3 m_Scale;	//スケール
	std::string m_Name;
	std::string m_Tag;
};