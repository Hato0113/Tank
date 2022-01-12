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
#include "Component\Enemy\EnemyAI\EnemyTaskManager.h"
#include "Component\Enemy\EnemyAI\EnemyTaskWeapon.h"

//-- 静的メンバ --
int EnemyManager::CurrentEnemy = 0;

void EnemyManager::Summon(DirectX::XMINT2 pos, EnemyType type)
{
	auto obj = Object::Create("Enemy");
	obj->transform->SetTag("Enemy");
	auto col = obj->AddComponent<SphereCollider>();
	col->SetRadius(5.0f);
	auto ai = obj->AddComponent<EnemyTaskManager>();
	auto wepTask = obj->AddComponent<EnemyTaskWeapon>();
	ai->AddTask(wepTask);
	wepTask->SetActive(false);

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
		wepTask->SetWeapon(wep);
	}
	break;
	case EnemyType::Quick:
	{
		auto em = obj->AddComponent<EnemyMovement>();
		em->Setting(pos, ResidentDataManager::GetData().EnemyData.Quickly,5);
		em->SetActive(false);
		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::Enemy02_Body));
		model->SetScale(5.0f);
		model->SetRelativePos({ 0.0f,-2.0f,0.0f });
		auto head = obj->AddComponent<Model>();
		head->SetModel(ModelManager::Get(ModelID::Enemy02_Head));
		head->SetUseParentRotate(false);
		head->SetScale(6.0f);
		head->SetRelativePos({ 0.0f,4.0f,0.0f });
		auto wep = obj->AddComponent<NormalWeapon>();
		wep->SetDiray(ResidentDataManager::GetData().EnemyData.ShotRate);
		wep->SetWeaponMode(WeaponMode::Auto);
		wep->SetActive(false);
		wep->SetTankHead(head);
		wepTask->SetWeapon(wep);
	}
	break;
	case EnemyType::Rapid:
	{
		auto em = obj->AddComponent<EnemyMovement>();
		em->Setting(pos);
		em->SetActive(false);
		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::Enemy03_Body));
		model->SetScale(5.0f);
		model->SetRelativePos({ 0.0f,-2.0f,0.0f });
		auto head = obj->AddComponent<Model>();
		head->SetModel(ModelManager::Get(ModelID::Enemy03_Head));
		head->SetUseParentRotate(false);
		head->SetScale(6.0f);
		head->SetRelativePos({ 0.0f,4.0f,0.0f });
		auto wep = obj->AddComponent<NormalWeapon>();
		wep->SetDiray(ResidentDataManager::GetData().EnemyData.RapidShotRate);
		wep->SetWeaponMode(WeaponMode::Auto);
		wep->SetActive(false);
		wep->SetTankHead(head);
		wepTask->SetWeapon(wep);
	}
	break;
	case EnemyType::Strong :
	{
		auto em = obj->AddComponent<EnemyMovement>();
		em->Setting(pos, ResidentDataManager::GetData().EnemyData.Quickly);
		em->SetActive(false);
		auto model = obj->AddComponent<Model>();
		model->SetModel(ModelManager::Get(ModelID::Enemy04_Body));
		model->SetScale(5.0f);
		model->SetRelativePos({ 0.0f,-2.0f,0.0f });
		auto head = obj->AddComponent<Model>();
		head->SetModel(ModelManager::Get(ModelID::Enemy04_Head));
		head->SetUseParentRotate(false);
		head->SetScale(6.0f);
		head->SetRelativePos({ 0.0f,4.0f,0.0f });
		auto wep = obj->AddComponent<NormalWeapon>();
		wep->SetDiray(ResidentDataManager::GetData().EnemyData.RapidShotRate);
		wep->SetWeaponMode(WeaponMode::Auto);
		wep->SetActive(false);
		wep->SetTankHead(head);
		wepTask->SetWeapon(wep);
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

bool EnemyManager::Kill(uint16_t kill)
{
	CurrentEnemy -= kill;
	if (CurrentEnemy == 0) return true;
	else return false;
}
