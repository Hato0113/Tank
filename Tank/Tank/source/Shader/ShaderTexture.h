/**
 * @brief シェーダー用テクスチャ
 * @author HatoriMasashi
 */
#pragma once

//-- include --
#include <d3d11.h>

//-- クラス定義 --
/**
 * @brief シェーダー用テクスチャ
 */
class ShaderManager;
class ShaderTexture final
{
	friend class ShaderManager;
public:
	/**
	 * @brief コンストラクタ
	 */
	ShaderTexture();

	/**
	 * @brief デストラクタ
	 */
	~ShaderTexture() = default;

public:
	/**
	 * @brief オブジェクトバインド
	 */
	void Bind();
	
	/**
	 * @brief セット
	 * 
	 * @param tex
	 * @param slot
	 */
	void Set(ID3D11ShaderResourceView* tex, int slot);
private:
	//! テクスチャリソース
	ID3D11ShaderResourceView* m_Tex;
	//! スロット
	int m_Slot;
};
