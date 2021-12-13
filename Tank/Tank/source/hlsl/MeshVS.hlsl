//-----------------------
//	Mesh用頂点シェーダー
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
	float3 Pos : POSITION;
	float4 Color : COLOR;
	float2 uv : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float2	TexCoord	: TEXCOORD2;
	float4	Diffuse		: COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 Position = float4(input.Pos, 1.0f);	//座標をfloat4に変換
	Position = mul(Position, mtxWorld);	//ワールド変換
	Position = mul(Position, mtxView);		//ビュー変換
	Position = mul(Position, mtxProjection);	//プロジェクション変換
	output.Position = Position;
	output.Pos4PS = mul(Position, mtxWorld).xyz;	//ワールド変換のみ
	output.Normal = mul(float4(input.Normal, 0.0f), mtxWorld).xyz;
	output.TexCoord = mul(float4(input.uv, 0.0f, 1.0f), mtxTexture).xy;
	output.Diffuse = input.Color;
	return output;
}