//------------------------------
//	Assimp�p���ʃs�N�Z���V�F�[�_�[
//-- author --
//  HatoriMasashi
//------------------------------
// 
// �O���[�o��
cbuffer CameraLight : register(b0) {
	matrix	mtxWVP;			// ���[���h�~�r���[�~�ˉe�s��
	matrix	mtxWorld;		// ���[���h�s��
	matrix  mtxView;		// �r���[�ύs��
	matrix  mtxProjection;	// �v���W�F�N�V�����ϊ��s��
	matrix	mtxTexture;		// �e�N�X�`���s��
	float4	vCameraPos;		// ���_���W(���[���h���)
	float4	vLightDir;		// ��������(���[���h���)
	float4	vLightAmbient;	// ����
	float4	vLightDiffuse;	// �g�U��
	float4	vLightSpecular;	// ���ʔ��ˌ�
};

// �}�e���A��
cbuffer material : register(b1) {
	float4	materialAmbient;			// ���F
	float4	materialDiffuse;			// �g�U�F+�A���t�@
	float4	materialSpecular;			// ���ʔ��ːF+���x
	float4	materialEmissive;			// �����F
	float4	materialFlags;			// �g�U�F�e�N�X�`���L��, �����F�e�N�X�`���L��, ���߃e�N�X�`���L��
};

Texture2D    g_texture			: register(t0);	// �e�N�X�`��
Texture2D    g_texEmissive		: register(t1);	// �����e�N�X�`��
Texture2D    g_texTransparent	: register(t2);	// ���߃e�N�X�`��
Texture2D    g_texSpecular		: register(t3);	// ���ʔ��˃e�N�X�`��
SamplerState g_sampler			: register(s0);	// �T���v��

// �p�����[�^
struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
};

//
// �s�N�Z���V�F�[�_
//
float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = materialDiffuse.rgb;
	float Alpha = materialDiffuse.a;
	
	//-- �e�N�X�`�����t�� --
	if (materialFlags.x != 0.0f) {		// �e�N�X�`���L��
		float4 TexDiff = g_texture.Sample(g_sampler, input.Tex);
		Diff *= TexDiff.rgb;
		Alpha *= TexDiff.a;
	}
	if (materialFlags.z != 0.0f) {		// �e�N�X�`���L��
		float4 TexTran = g_texTransparent.Sample(g_sampler, input.Tex);
		Alpha *= (TexTran.r * TexTran.g * TexTran.b * TexTran.a);
	}
	if (Alpha <= 0.0f) discard;		// ���S�����Ȃ�`�悵�Ȃ�
	float3 Spec = materialSpecular.rgb;
	if (materialFlags.w != 0.0f) {		// �e�N�X�`���L��
		float4 TexSpec = g_texSpecular.Sample(g_sampler, input.Tex);
		Spec *= TexSpec.rgb;
	}

	if (vLightDir.x != 0.0f || vLightDir.y != 0.0f || vLightDir.z != 0.0f) {
		float3 L = normalize(-vLightDir.xyz);					// �����ւ̃x�N�g��
		float3 N = normalize(input.Normal);						// �@���x�N�g��

		float3 V = normalize(vCameraPos.xyz - input.PosForPS);// ���_�ւ̃x�N�g��
		float3 H = normalize(L + V);							// �n�[�t�x�N�g��
		float val = saturate(dot(L, N)) * 0.4f + 0.6f;
		Diff = vLightAmbient.rgb * materialAmbient.rgb +
			vLightDiffuse.rgb * Diff * val * val;
		Spec = Spec * vLightSpecular.rgb *
			pow(saturate(dot(N, H)), materialSpecular.a);				// ���ʔ��ːF
		Diff += Spec;
	}

	float3 Emis = materialEmissive.rgb;
	if (materialFlags.y != 0.0f) {		// �e�N�X�`���L��
		float4 TexEmis = g_texEmissive.Sample(g_sampler, input.Tex);
		Emis *= (TexEmis.rgb * TexEmis.a);
	}
	Diff += Emis;

	return float4(Diff, Alpha);
}
