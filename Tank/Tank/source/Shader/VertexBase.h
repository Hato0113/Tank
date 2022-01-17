/**
 * @brief 頂点シェーダーベース
 * @author HatoriMasashi
 */
#pragma once

//-- include --
#include <d3d11.h>
#include <string>

//-- クラス定義 --
/**
 * @brief 頂点シェーダーベース
 */
class VertexBase abstract
{
public:
	/**
	 * @brief コンストラクタ
	 */
	VertexBase();

	/**
	 * @brief デストラクタ
	 */
	virtual ~VertexBase();
public:
	
	/**
	 * @brief バインド
	 */
	virtual void Bind() final;

	/**
	 * @brief 生成関数
	 * 
	 * @param FileName
	 * @return result
	 */
	virtual HRESULT Make(std::string) = 0;

protected:
	//! 頂点シェーダー
	ID3D11VertexShader* m_Shader;
	//! 頂点レイアウト
	ID3D11InputLayout* m_Layout;
};

