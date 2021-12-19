//--------------------------
//	常駐するデバッグフラグ
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

/*
	IMGUIで操作するフラグ関連
*/

//-- 構造体定義 --
struct ResidentFlag
{
	struct
	{
		bool Pause = false;			//ポーズ
		bool ShowCollider = false;	//コライダー可視化
	}SystemFlag;
	struct
	{
		struct
		{
			bool Invincible = false;		//無敵フラグ
			bool AutoShot = false;			//自動射撃
			bool InfiniteBullet = false;	//弾無限
		}Player;
		struct
		{
			bool Move = true;	//敵行動
			bool Shot = true;	//敵射撃
			bool ShowTarget = false;
		}Enemy;
	}GamePlay;
};

//-- クラス定義 --
class ResidentFlagManager
{
public:
	static ResidentFlag GetData() { return m_Data; }
	static void SetData(ResidentFlag data) { m_Data = data; }

private:
	static ResidentFlag m_Data;
};



