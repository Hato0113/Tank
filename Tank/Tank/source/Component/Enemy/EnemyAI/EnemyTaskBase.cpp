//-------------------------
//	敵AIベース
//-- author --
//	HatoriMasashi
//-------------------------

//-- include --
#include "EnemyTaskBase.h"

/*
	コンストラクタ
*/
EnemyTaskBase::EnemyTaskBase()
{
	m_End = false;
	m_RemainTime = -1;
}

/*
	終了チェック関数
	return : bool 終わったか否か
*/
bool EnemyTaskBase::CheckEnd()
{
	return m_End;
}
