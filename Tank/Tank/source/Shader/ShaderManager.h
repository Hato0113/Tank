/**
 * @brief シェーダーマネージャー
 * @author HatoriMasashi
 */
#pragma once

//-- include --
#include <d3d11.h>
#include <unordered_map>
#include <vector>
#include "VertexBase.h"
#include "PixelBase.h"
#include "ConstantBuffer.h"
#include "ShaderTexture.h"

//-- クラス定義 --
/**
 * @brief シェーダーマネージャー
 * @details シングルトン
 */
class ShaderManager
{
private:
	/**
	 * @brief コンストラクタ
	 */
	ShaderManager();

	/**
	 * @brief デストラクタ
	 */
	~ShaderManager() = default;

public:
	/**
	 * @brief インスタンス取得
	 * @return シェーダーマネージャーオブジェクト参照
	 */
	static ShaderManager& GetInstance()
	{
		static ShaderManager inst;
		return inst;
	}

private:
	/**
	 * @brief 頂点シェーダーセット
	 * 
	 * @param name
	 * @param 頂点シェーダーオブジェクトポインタ
	 */
	void Set(std::string name, VertexBase*);

	/**
	 * @brief ピクセルシェーダーセット
	 * 
	 * @param name
	 * @param ピクセルシェーダーオブジェクトポインタ
	 */
	void Set(std::string name, PixelBase*);

	/**
	 * @brief 定数バッファセット
	 * 
	 * @param name
	 * @param 定数バッファオブジェクトポインタ
	 */
	void Set(std::string name, ConstantBuffer*);

	/**
	 * @brief シェーダーリソースセット
	 * 
	 * @param name
	 * @param シェーダー用テクスチャオブジェクトポインタ
	 */
	void Set(std::string name, ShaderTexture*);

public:
	/**
	 * @brief 定数バッファバインド
	 */
	void ConstantBufferBind();

	/**
	 * @brief テクスチャリソースバインド
	 */
	void TextureResourceBind();
	
	/**
	 * @brief テクスチャリソースバインド
	 * 
	 * @param name
	 * @param data
	 */
	void ConstantWrite(std::string name, void* data);

	/**
	 * @brief 初期化
	 */
	void Init();
	
	/**
	 * @brief 終了処理
	 */
	void Uninit();

	/**
	 * @brief 頂点シェーダーバインド
	 * 
	 * @param name
	 */
	void BindVS(std::string name);

	/**
	 * @brief ピクセルシェーダーバインド
	 * 
	 * @param name
	 */
	void BindPS(std::string name);

private:
	//! 頂点シェーダー連想配列
	std::unordered_map<std::string, VertexBase*> VertexMap;
	//! ピクセルシェーダー連想配列
	std::unordered_map<std::string, PixelBase*> PixelMap;
	//! 定数バッファ連想配列
	std::unordered_map<std::string, ConstantBuffer*> ConstantMap;
	//! テクスチャリソース連想配列
	std::unordered_map<std::string, ShaderTexture*> TextureMap;
};


