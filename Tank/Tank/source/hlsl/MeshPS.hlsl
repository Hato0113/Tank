//-----------------------
//	Mesh�p���_�V�F�[�_�[
//-- author --
//	HatoriMasashi
//-----------------------

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

cbuffer material : register(b1) {
	float4	materialAmbient;			// ���F
	float4	materialDiffuse;			// �g�U�F+�A���t�@
	float4	materialSpecular;			// ���ʔ��ːF+���x
	float4	materialEmissive;			// �����F
	float4	materialFlags;			// �g�U�F�e�N�X�`���L��, �����F�e�N�X�`���L��, ���߃e�N�X�`���L��
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
	//-- �e�N�X�`���L�� --
	if (materialAmbient.a > 0.0f)
	{
		float4 TexDiffuse = g_Texture.Sample(g_Sampler, input.TexCoord);
		Diffuse *= TexDiffuse.rgb;
		Alpha *= TexDiffuse.a;
	}
	if (Alpha <= 0.0f) discard;

	if (vLightDir.x != 0.0f || vLightDir.y != 0.0f || vLightDir.z != 0.0f)
	{
		//-- �������L�� --
		float3 L = normalize(-vLightDir.xyz);				// �����ւ̃x�N�g��
		float3 N = normalize(input.Normal);					// �@���x�N�g��
		float3 V = normalize(vCameraPos.xyz - input.Pos4PS);	// ���_�ւ̃x�N�g��
		float3 H = normalize(L + V);						// �n�[�t�x�N�g��
		float val = saturate(dot(L, N)) * 0.4f + 0.6f;
		Diffuse = vLightAmbient.rgb * materialAmbient.rgb + materialDiffuse.rgb *
			Diffuse * val * val;						// �g�U�F + ���F
		float3 Spec = materialSpecular.rgb * vLightSpecular.rgb *
			pow(saturate(dot(N, H)), materialSpecular.a);				// ���ʔ��ːF
		Diffuse += Spec;
	}

	Diffuse += materialEmissive.rgb;
	return float4(Diffuse, Alpha);

}