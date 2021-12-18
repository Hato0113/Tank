//--------------------------
//	武器(装備クラス)
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "Bullet\BulletManager.h"

//-- enum --
enum class WeaponMode
{
	None,
	OpSelf,
	Auto,
};

//-- クラス定義 --
class WeaponBase : public Component
{
public:
	WeaponBase();
	virtual ~WeaponBase() = default;

public:
	virtual void Update() = 0;
	void SetWeaponMode(WeaponMode mode) { m_Mode = mode; }
	DirectX::XMFLOAT3 GetDir() { return m_Dir; }
	void SetTankHead(Model* head) { m_Head = head; }
	  
protected:
	void UpdateDirSelf();
	void UpdateDirByMouse();	//マウスでのターゲット制御
	void TargetPlayer();
	void SetBulletType(BulletType type) { m_BulletType = type; }

private:
	void RotHead();	//砲身の回転

protected:
	DirectX::XMFLOAT3 m_Dir;
	WeaponMode m_Mode;
	BulletType m_BulletType;
private:
	Quaternion m_Start;
	Quaternion m_Target;
	Quaternion m_Current;
	float m_CurrentTheta;
	float m_Time;
	float m_Ratio;
	Model* m_Head;
};