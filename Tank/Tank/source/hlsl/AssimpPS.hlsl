//------------------------------
//	Assimp用共通ピクセルシェーダー
//-- author --
//  HatoriMasashi
//------------------------------
// 
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

// マテリアル
cbuffer material : register(b1) {
	float4	materialAmbient;			// 環境色
	float4	materialDiffuse;			// 拡散色+アルファ
	float4	materialSpecular;			// 鏡面反射色+強度
	float4	materialEmissive;			// 発光色
	float4	materialFlags;			// 拡散色テクスチャ有無, 発光色テクスチャ有無, 透過テクスチャ有無
};

Texture2D    g_texture			: register(t0);	// テクスチャ
Texture2D    g_texEmissive		: register(t1);	// 発光テクスチャ
Texture2D    g_texTransparent	: register(t2);	// 透過テクスチャ
Texture2D    g_texSpecular		: register(t3);	// 鏡面反射テクスチャ
SamplerState g_sampler			: register(s0);	// サンプラ

// パラメータ
struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
};

//
// ピクセルシェーダ
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = materialDiffuse.rgb;
	float Alpha = materialDiffuse.a;
	
	//-- テクスチャ情報付加 --
	if (materialFlags.x != 0.0f) {		// テクスチャ有無
		float4 TexDiff = g_texture.Sample(g_sampler, input.Tex);
		Diff *= TexDiff.rgb;
		Alpha *= TexDiff.a;
	}
	if (materialFlags.z != 0.0f) {		// テクスチャ有無
		float4 TexTran = g_texTransparent.Sample(g_sampler, input.Tex);
		Alpha *= (TexTran.r * TexTran.g * TexTran.b * TexTran.a);
	}
	if (Alpha <= 0.0f) discard;		// 完全透明なら描画しない
	float3 Spec = materialSpecular.rgb;
	if (materialFlags.w != 0.0f) {		// テクスチャ有無
		float4 TexSpec = g_texSpecular.Sample(g_sampler, input.Tex);
		Spec *= TexSpec.rgb;
	}

	if (vLightDir.x != 0.0f || vLightDir.y != 0.0f || vLightDir.z != 0.0f) {
		float3 L = normalize(-vLightDir.xyz);					// 光源へのベクトル
		float3 N = normalize(input.Normal);						// 法線ベクトル

		float3 V = normalize(vCameraPos.xyz - input.PosForPS);// 視点へのベクトル
		float3 H = normalize(L + V);							// ハーフベクトル
		float val = saturate(dot(L, N)) * 0.4f + 0.6f;
		Diff = vLightAmbient.rgb * materialAmbient.rgb +
			vLightDiffuse.rgb * Diff * val * val;
		Spec = Spec * vLightSpecular.rgb *
			pow(saturate(dot(N, H)), materialSpecular.a);				// 鏡面反射色
		Diff += Spec;
	}

	float3 Emis = materialEmissive.rgb;
	if (materialFlags.y != 0.0f) {		// テクスチャ有無
		float4 TexEmis = g_texEmissive.Sample(g_sampler, input.Tex);
		Emis *= (TexEmis.rgb * TexEmis.a);
	}
	Diff += Emis;

	return float4(Diff, Alpha);
}
