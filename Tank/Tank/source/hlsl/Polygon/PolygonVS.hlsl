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

