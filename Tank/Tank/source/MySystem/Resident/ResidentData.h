//--------------------------
//	常駐するデータ
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

/*
	ゲーム内各所のステータスを管理。
	IMGUIでステータスをいじれるようにする。
	初期値として設定するものをセーブデータとして書き出す。
*/
//-- include --
#include <DirectXMath.h>
#include <string>

//-- 構造体定義 --
struct ResidentData
{
	struct
	{
		float MaxSpeed = 0.3f;		//スピード
		float AccFrame = 3.0f;		//加速フレーム
		float DecFrame = 5.0f;		//減速フレーム
		int ShotRate = 10;		//ショットのレート
		int ShotMax = 5;		//最大弾数
	}PlayerData;
	struct
	{
		float Speed = 0.3f;
		int ShotRate = 120;
	}EnemyData;
};

//-- クラス定義 --
class ResidentDataManager
{
public:
	static void Init();
	static void Uninit();
	static void Load();
	static void Save();

	static ResidentData GetData() { return m_Data; }
	static void SetData(ResidentData data) { m_Data = data; }

private:
	static ResidentData m_Data;
};

