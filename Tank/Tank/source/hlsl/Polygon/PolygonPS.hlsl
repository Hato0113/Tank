//-----------------------
//	�|���S���p���_�V�F�[�_�[
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

struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

Texture2D g_tex : register(t0);
SamplerState g_sampler : register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 color = input.Diffuse;
	if (mtxTexture._44 > 0.0f)
	{
		color *= g_tex.Sample(g_sampler, input.TexCoord);
	}
	return color;
}