/**
 * @brief ピクセルシェーダーベース
 * @author HatoriMasashi
 */
#pragma once

//-- include --
#include <d3d11.h>
#include <string>

//-- クラス定義 --
/**
 * @brief ピクセルシェーダーベース
 */
class PixelBase final
{
public:
	/**
	 * @brief コンストラクタ
	 */
	PixelBase();

	/**
	 * @brief デストラクタ
	 */
	~PixelBase();

public:
	/**
	 * @brief バインド
	 */
	void Bind();

	/**
	 * @brief 生成
	 * 
	 * @param fileName
	 */
	HRESULT Make(std::string fileName);

private:
	//! ピクセルシェーダー
	ID3D11PixelShader* m_Shader;
};
