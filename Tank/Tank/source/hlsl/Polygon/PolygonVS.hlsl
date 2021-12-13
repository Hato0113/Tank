//-----------------------
//	ポリゴン用頂点シェーダー
//-- author --
//	HatoriMasashi
//-----------------------

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

struct VS_INPUT
{
	float3	Pos	: POSITION;
	float2	UV	: TEXCOORD0;
	float4	Diffuse	: COLOR0;
};

struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 pos = mul(float4(input.Pos, 1.0f), mtxWorld);
	pos = mul(pos, mtxView);
	output.Position = mul(pos, mtxProjection);
	output.TexCoord = mul(float4(input.UV, 0.0f, 1.0f), mtxTexture).xy;
	output.Diffuse = input.Diffuse;
	return output;
}

