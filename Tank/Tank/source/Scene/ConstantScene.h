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
	void Init() override;
	void Update() override;
	void Draw() override;
	void Uninit() override;

	static ConstantScene& GetInstance()
	{
		static ConstantScene inst;
		return inst;
	}
};


