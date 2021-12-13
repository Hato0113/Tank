//-----------------------------
//	定数バッファ生成
//-- author --
//	HatoriMasashi
//-----------------------------

#pragma once

//-- include --
#include <d3d11.h>
#include <string>

//-- enum --
enum class ConstantType
{
	Vertex = 0,
	Pixel,
	Both,
	max
};

//-- クラス定義 --
class ShaderManager;
class ConstantBuffer final
{
	friend class ShaderManager;
public:
	ConstantBuffer();
	~ConstantBuffer();
public:
	void Bind();
	HRESULT Make(size_t size, int slot, ConstantType type);
private:
	ID3D11Buffer* m_Buffer;
	int m_Slot;
	ConstantType m_Type;
};
