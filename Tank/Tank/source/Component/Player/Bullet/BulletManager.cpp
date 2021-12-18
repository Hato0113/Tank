//--------------------------
//	弾マネージャ
//-- author --
//	HatoriMasashi
//--------------------------

//-- include --
#include "BulletManager.h"
#include "Scene\SceneManager.h"
using namespace DirectX;
#include "IMGUI\GUI_Message.h"
#include "NormalBullet.h"
#include "Component\Model\Model.h"
#include "Component\Collider\SphereCollider.h"

void BulletManager::Summon(XMFLOAT3 pos, XMFLOAT3 dir, BulletType type,ModelID modelID, NormalWeapon* pare)
{
	auto obj = Object::Create("Bullet");
	obj->transform->SetTag("Bullet");
	auto col = obj->AddComponent<SphereCollider>();
	col->SetRadius(3.0f);
	col->SetSubjectState(true);

	switch (type)
	{
	case BulletType::None:
		break;
	case BulletType::Normal:
	{
		obj->transform->SetPos(pos);
		//obj->SetLifeTime(500);
		auto bullet = obj->AddComponent<NormalBullet>();
		bullet->m_Dir = dir;
		bullet->SetInvalidTime(10);
		bullet->SetParentWeapon(pare);

		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(modelID));
		model->SetScale(2.0f);
		//向き変更
		float theta = atan2f(dir.z, dir.x);
		theta -= DirectX::XM_PIDIV2;
		theta *= -1;
		Quaternion q;
		q.SetQuaternion(obj->transform->GetEulerAngle());
		q.SetToRotateAxisAngle(Quaternion::Up, theta);
		EulerAngles ea;
		ea.SetEulerAngles(q);
		obj->transform->SetRotate(ea);
		bullet->SetSphereCollider(col);

	}
	break;

	default:
		break;
	}

	auto scene = SceneManager::GetInstance().GetCurrentScene();

	scene->manager->Add(obj);
}

