//--------------------------
//	アプリケーション管理
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- クラス定義 --
namespace tank
{
	class Application;
}
class tank::Application
{
public:
	/*
		初期化
		パラメータ無し
	*/
	static void Init();
	/*
		更新
		パラメータ無し
	*/
	static void Update();
	/*
		描画
		パラメータ無し
	*/
	static void Draw();
	/*
		終了処理
		パラメータ無し
	*/
	static void Uninit();
	/*
		ゲーム終了
		パラメータ無し
	*/
	static void End() { m_GameFlag = false; }
	/*
		ゲームフラグ取得
		戻り値 : フラグ
	*/
	static bool GetGameFlag() { return m_GameFlag;}
	/*
		ゲームフラグセット
		引数 : フラグ
	*/
	static void SetGameFlag(bool state) { m_GameFlag = state; }
	
	//-- ゲーム用変数 --
private:
	static bool m_GameFlag; //ゲームフラグ
};
