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
	Both, //頂点 / ピクセルシェーダーの両方に送る
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
	/*
		バインド関数
		パラメータ無し
	*/
	void Bind();

	/*
		バッファ作成関数
		引数 : データサイズ,スロット,定数バッファタイプ
	*/
	HRESULT Make(size_t size, int slot, ConstantType type);
private:
	ID3D11Buffer* m_Buffer;
	int m_Slot;
	ConstantType m_Type;
};
