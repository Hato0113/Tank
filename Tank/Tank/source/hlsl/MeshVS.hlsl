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
	float4 Position = float4(input.Pos, 1.0f);	//���W��float4�ɕϊ�
	Position = mul(Position, mtxWorld);	//���[���h�ϊ�
	Position = mul(Position, mtxView);		//�r���[�ϊ�
	Position = mul(Position, mtxProjection);	//�v���W�F�N�V�����ϊ�
	output.Position = Position;
	output.Pos4PS = mul(Position, mtxWorld).xyz;	//���[���h�ϊ��̂�
	output.Normal = mul(float4(input.Normal, 0.0f), mtxWorld).xyz;
	output.TexCoord = mul(float4(input.uv, 0.0f, 1.0f), mtxTexture).xy;
	output.Diffuse = input.Color;
	return output;
}