//--------------------------
//	アプリケーション管理
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- クラス定義 --
namespace node
{
	class Application;
}
class node::Application
{
public:
	static void Init();
	static void Update();
	static void Draw();
	static void Uninit();
	static void End() { m_GameFlag = false; }
	static bool GetGameFlag() { return m_GameFlag;}	
	static void SetGameFlag(bool state) { m_GameFlag = state; }
	
	//-- ゲーム用変数 --
private:
	static bool m_GameFlag; //ゲームフラグ
};
