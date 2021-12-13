//--------------------------
//	Effekseerエフェクト管理
//-- author --
//	HatoriMasashi
//--------------------------
#pragma once

//-- include --
#include "Effekseer\Effekseer.h"
#include "Effekseer\EffekseerRendererDX11.h"
#include <unordered_map>
#include "EffectID.h"

#pragma comment(lib, "source\\MySystem\\Effect\\Effekseer\\Effekseer.lib")
#pragma comment(lib, "source\\MySystem\\Effect\\Effekseer\\EffekseerRendererDX11.lib")

//-- クラス定義 --
class Effect;
class EffectManager
{
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static ::Effekseer::EffectRef Get(EffectID id);
	static void Load(EffectID id, std::u16string fileName);

	static ::EffekseerRendererDX11::RendererRef m_Renderer;
	static ::Effekseer::ManagerRef m_Manager;

private:
	static std::unordered_map<EffectID, ::Effekseer::EffectRef> m_EffectMap;
};

