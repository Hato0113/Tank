//------------------------------
//	常駐シーン
//-- author --
//	HatoriMasashi
//------------------------------
#pragma once

//-- include --
#include "Scene\SceneBase.h"

//-- クラス定義 --
class ConstantScene final : public SceneBase
{
private:
	ConstantScene();
	~ConstantScene() = default;
public:
	/*
		初期化
		パラメータ無し
	*/
	void Init() override;

	/*
		更新
		パラメータ無し
	*/
	void Update() override;
	/*
		描画
		パラメータ無し
	*/
	void Draw() override;
	/*
		終了処理
		パラメータ無し
	*/
	void Uninit() override;

	/*
		インスタンス取得関数
		戻り値 : インスタンスへの参照
	*/
	static ConstantScene& GetInstance()
	{
		static ConstantScene inst;
		return inst;
	}
};


