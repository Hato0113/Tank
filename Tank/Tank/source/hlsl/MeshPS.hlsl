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

cbuffer material : register(b1) {
	float4	materialAmbient;			// 環境色
	float4	materialDiffuse;			// 拡散色+アルファ
	float4	materialSpecular;			// 鏡面反射色+強度
	float4	materialEmissive;			// 発光色
	float4	materialFlags;			// 拡散色テクスチャ有無, 発光色テクスチャ有無, 透過テクスチャ有無
};

struct VS_OUTPUT
{
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float2	TexCoord	: TEXCOORD2;
	float4	Diffuse		: COLOR0;
};

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float3 Diffuse = input.Diffuse.rgb * materialDiffuse.rgb;
	float Alpha = input.Diffuse.a * materialDiffuse.a;
	//-- テクスチャ有無 --
	if (materialAmbient.a > 0.0f)
	{
		float4 TexDiffuse = g_Texture.Sample(g_Sampler, input.TexCoord);
		Diffuse *= TexDiffuse.rgb;
		Alpha *= TexDiffuse.a;
	}
	if (Alpha <= 0.0f) discard;

	if (vLightDir.x != 0.0f || vLightDir.y != 0.0f || vLightDir.z != 0.0f)
	{
		//-- 光源が有効 --
		float3 L = normalize(-vLightDir.xyz);				// 光源へのベクトル
		float3 N = normalize(input.Normal);					// 法線ベクトル
		float3 V = normalize(vCameraPos.xyz - input.Pos4PS);	// 視点へのベクトル
		float3 H = normalize(L + V);						// ハーフベクトル
		float val = saturate(dot(L, N)) * 0.4f + 0.6f;
		Diffuse = vLightAmbient.rgb * materialAmbient.rgb + materialDiffuse.rgb *
			Diffuse * val * val;						// 拡散色 + 環境色
		float3 Spec = materialSpecular.rgb * vLightSpecular.rgb *
			pow(saturate(dot(N, H)), materialSpecular.a);				// 鏡面反射色
		Diffuse += Spec;
	}

	Diffuse += materialEmissive.rgb;
	return float4(Diffuse, Alpha);

}