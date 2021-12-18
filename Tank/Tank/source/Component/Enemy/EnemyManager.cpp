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
	col->SetRadius(5.0f);

	switch (type)
	{
	case EnemyType::Normal:
	{
		auto em = obj->AddComponent<EnemyMovement>();
		em->Setting(pos);
		em->SetActive(false);
		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::Enemy01_Body));
		model->SetScale(5.0f);
		model->SetRelativePos({ 0.0f,-2.0f,0.0f });
		auto head = obj->AddComponent<Model>();
		head->SetModel(ModelManager::Get(ModelID::Enemy01_Head));
		head->SetUseParentRotate(false);
		head->SetScale(6.0f);
		head->SetRelativePos({ 0.0f,4.0f,0.0f });
		auto wep = obj->AddComponent<NormalWeapon>();
		wep->SetDiray(ResidentDataManager::GetData().EnemyData.ShotRate);
		wep->SetWeaponMode(WeaponMode::Auto);
		wep->SetActive(false);
		wep->SetTankHead(head);
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
