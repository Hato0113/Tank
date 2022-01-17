/**
 * @brief 定数バッファ定義
 * @details	シェーダーで使用する定数バッファの構造体用ヘッダ
	        ここに書き込んだ構造体のバッファをShaderManagerで生成する。
 */
#pragma once

//-- include --
#include <DirectXMath.h>

//-- 構造体定義 --
/**
 * @brief カメラライト情報
 */
struct CB_CameraLight
{
	DirectX::XMMATRIX	WVPMat;		//ワールド×ビュー×射影行列(転置行列)
	DirectX::XMMATRIX   WorldMat;	//ワールド行列(転置行列)
	DirectX::XMMATRIX	View;		//ビュー変換行列
	DirectX::XMMATRIX	Projection;	//プロジェクション変換行列
	DirectX::XMMATRIX	TextureMat;	//テクスチャ行列(転置行列)
	DirectX::XMVECTOR	EyePos;		//視点座標
	DirectX::XMVECTOR	LightDir;	//光源方向(ライトが向いている方向)
	DirectX::XMVECTOR	LightAmbient;	//環境光
	DirectX::XMVECTOR	LightDiffuse;	//ライト色
	DirectX::XMVECTOR	LightSpecular;	//鏡面反射光
};

/**
 * @brief マテリアル情報
 */
struct CB_Material
{
	DirectX::XMVECTOR	Ambient;	// アンビエント色
	DirectX::XMVECTOR	Diffuse;	// ディフューズ色
	DirectX::XMVECTOR	Specular;	// スペキュラ色
	DirectX::XMVECTOR	Emissive;	// エミッシブ色
	DirectX::XMVECTOR	Flags;		// テクスチャ有無
};

/**
 * @brief Assimp用ボーン情報
 */
struct CB_AssimpBone
{
	static constexpr int BoneMax = 64;
	DirectX::XMMATRIX mBone[BoneMax];
	CB_AssimpBone()
	{
		for (int i = 0; i < BoneMax; i++) {
			mBone[i] = DirectX::XMMatrixIdentity();
		}
	}
};
