//----------------------------------
// シェーダー用テクスチャ
//-- author --
//	HatoriMasashi
//----------------------------------
#pragma once

//-- include --
#include <d3d11.h>

//-- クラス定義 --
class ShaderManager;
class ShaderTexture final
{
	friend class ShaderManager;
public:
	ShaderTexture();
	~ShaderTexture() = default;

public:
	void Bind();
	void Set(ID3D11ShaderResourceView* tex, int slot);
private:
	ID3D11ShaderResourceView* m_Tex;
	int m_Slot;
};
