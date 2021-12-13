//------------------------------
//	Assimp用共通頂点シェーダー
//-- author --
//  HatoriMasashi
//------------------------------
#define MAX_BONE_MATRIX	64

// グローバル
cbuffer CameraLight : register(b0) {
	matrix	mtxWVP;			// ワールド×ビュー×射影行列
	matrix	mtxWorld;		// ワールド行列
	matrix  mtxView;		// ビュー変行列
	matrix  mtxProjection;	// プロジェクション変換行列
	matrix	mtxTexture;		// テクスチャ行列
	float4	vCameraPos;		// 視点座標(ワールド空間)
	float4	vLightDir;		// 光源方向(ワールド空間)
	float4	vLightAmbient;	// 環境光
	float4	vLightDiffuse;	// 拡散光
	float4	vLightSpecular;	// 鏡面反射光
};

// ボーンのポーズ行列
cbuffer global_bones : register(b2) {
	matrix g_BoneWorld[MAX_BONE_MATRIX];
};

// パラメータ
struct VS_INPUT {
	float3	Pos		: POSITION;
	float3	Normal	: NORMAL;
	float2	Tex		: TEXCOORD0;
	float3  Tangent : TANGENT0;
	uint4	Bone	: BONE_INDEX;	// ボーンのインデックス
	float4	Weight	: BONE_WEIGHT;	// ボーンの重み
};

struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
	float3  TexSpaceLight : TEXCOORD3;
};

// スキニング後の頂点・法線
struct SKIN {
	float4 Pos;
	float3 Norm;
};

// 頂点をスキニング (ボーンにより移動)
SKIN SkinVert(VS_INPUT input)
{
	SKIN output;
	float4 Pos = float4(input.Pos, 1.0f);
	float3 Norm = input.Normal;
	if (input.Weight.x == 0.0f) {
		// ボーン無し
		output.Pos = Pos;
		output.Norm = Norm;
	} else {
		// ボーン0
		uint uBone = input.Bone.x;
		float fWeight = input.Weight.x;
		matrix m = g_BoneWorld[uBone];
		output.Pos = fWeight * mul(Pos, m);
		output.Norm = fWeight * mul(Norm, (float3x3)m);
		// ボーン1
		uBone = input.Bone.y;
		fWeight = input.Weight.y;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
		// ボーン2
		uBone = input.Bone.z;
		fWeight = input.Weight.z;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
		// ボーン3
		uBone = input.Bone.w;
		fWeight = input.Weight.w;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
	}
	return output;
}

//
// 頂点シェーダ
//
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	SKIN vSkinned = SkinVert(input);
	output.Pos = mul(vSkinned.Pos, mtxWVP);
	output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), mtxTexture).xy;
	output.Normal = mul(vSkinned.Norm, (float3x3)mtxWorld);
	output.PosForPS = mul(vSkinned.Pos, mtxWorld).xyz;

	//-- 接ベクトル計算 --
	float3 N = normalize(input.Normal);
	float3 T = normalize(input.Tangent);
	float3 B = normalize(cross(N, T));

	float3x3 invTexToWorld =
		float3x3(
			T.x, B.x, N.x,
			T.y, B.y, N.y,
			T.z, B.z, N.z
			);
	//ライトをテクスチャ空間に変換
	float3 L = normalize(vLightDir.xyz);
	output.TexSpaceLight = mul(L, invTexToWorld);


	return output;
}
