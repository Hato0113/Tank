//------------------------
// ロード画面
//-- author --
//	HatoriMasashi
//------------------------
#pragma once

//-- include --
#include "Component\Component.h"
#include "MySystem\Texture\TextureManager.h"
#include "Component\Polygon\Polygon.h"

//-- クラス定義 --
class Loading : public Component
{
public:
	void Update()
	{
		if (!poly)
		{
			poly = parent->GetComponent<CPolygon>();
		}

		count++;
		if (count > Max)
		{
			current++;
			if (current == 3)
				current = 0;
			count = 0;
		}

		switch (current)
		{
		case 0:
			poly->SetTex(TextureManager::Get(TextureID::Loading01));
			break;
		case 1:
			poly->SetTex(TextureManager::Get(TextureID::Loading02));
			break;
		case 2:
			poly->SetTex(TextureManager::Get(TextureID::Loading03));
			break;
		default:
			break;
		}
	}
private:
	CPolygon* poly = nullptr;
	int current = 0;
	int count = 0;
	const int Max = 30;
};
