//------------------------
// 敵マネージャ
//-- author --
//	HatoriMasashi
//------------------------

//-- include --
#include "EnemyManager.h"
#include "Component\Model\Model.h"
#include "Component\Collider\SphereCollider.h"
#include "Scene\SceneManager.h"
#include "Component\Player\Weapon\NormalWeapon.h"
#include "Component\Enemy\EnemyMovement.h"
#include "MySystem\Resident\ResidentData.h"

//-- 静的メンバ --
int EnemyManager::CurrentEnemy = 0;

void EnemyManager::Summon(DirectX::XMINT2 pos, EnemyType type)
{
	auto obj = Object::Create("Enemy");
	obj->transform->SetTag("Enemy");
	auto col = obj->AddComponent<SphereCollider>();
	col->SetRadius(3.0f);

	switch (type)
	{
	case EnemyType::Normal:
	{
		auto em = obj->AddComponent<EnemyMovement>();
		em->Setting(pos);
		em->SetActive(false);
		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::EnemyPlane));
		model->SetScale(4.0f);
		auto wep = obj->AddComponent<NormalWeapon>();
		wep->SetDiray(ResidentDataManager::GetData().EnemyData.ShotRate);
		wep->SetWeaponMode(WeaponMode::Auto);
		wep->SetActive(false);
	}
	break;
	default:
		break;
	}

	auto scene = SceneManager::GetInstance().GetCurrentScene();

	scene->manager->Add(obj);

	EnemyManager::Add();
}

void EnemyManager::Add(uint16_t add)
{
	CurrentEnemy += add;
}

void EnemyManager::Kill(uint16_t kill)
{
	CurrentEnemy -= kill;
	if (CurrentEnemy == 0)
	{
		SceneManager::GetInstance().SetNextChange(SceneType::Title);
	}
}
