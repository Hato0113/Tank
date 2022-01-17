/**
 * @brief 定数バッファ
 * @author HatoriMasashi
 */

#pragma once

//-- include --
#include <d3d11.h>
#include <string>

//-- enum --
/**
 * @brief 定数バッファのタイプ
 * @details 送るシェーダーの種類
 */
enum class ConstantType
{
	Vertex = 0,
	Pixel,
	Both, //頂点 / ピクセルシェーダーの両方に送る
	max
};

//-- クラス定義 --
/**
 * 定数バッファオブジェクトクラス
 */
class ShaderManager;
class ConstantBuffer final
{
	friend class ShaderManager;
public:
	/**
	 * @brief　コンストラクタ
	 */
	ConstantBuffer();

	/**
	 * @brief デストラクタ
	 */
	~ConstantBuffer();
public:
	/**
	 * @brief バインド
	 */
	void Bind();

	/**
	 * @brief 生成
	 * 
	 * @param size
	 * @param slot
	 * @param type
	 * @return result
	 */
	HRESULT Make(size_t size, int slot, ConstantType type);
private:
	//! バッファ
	ID3D11Buffer* m_Buffer;
	//! スロット
	int m_Slot;
	//! タイプ
	ConstantType m_Type;
};
