//------------------------------
//	Assimp�p���ʒ��_�V�F�[�_�[
//-- author --
//  HatoriMasashi
//------------------------------
#define MAX_BONE_MATRIX	64

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

// �{�[���̃|�[�Y�s��
cbuffer global_bones : register(b2) {
	matrix g_BoneWorld[MAX_BONE_MATRIX];
};

// �p�����[�^
struct VS_INPUT {
	float3	Pos		: POSITION;
	float3	Normal	: NORMAL;
	float2	Tex		: TEXCOORD0;
	float3  Tangent : TANGENT0;
	uint4	Bone	: BONE_INDEX;	// �{�[���̃C���f�b�N�X
	float4	Weight	: BONE_WEIGHT;	// �{�[���̏d��
};

struct VS_OUTPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
	float3  TexSpaceLight : TEXCOORD3;
};

// �X�L�j���O��̒��_�E�@��
struct SKIN {
	float4 Pos;
	float3 Norm;
};

// ���_���X�L�j���O (�{�[���ɂ��ړ�)
SKIN SkinVert(VS_INPUT input)
{
	SKIN output;
	float4 Pos = float4(input.Pos, 1.0f);
	float3 Norm = input.Normal;
	if (input.Weight.x == 0.0f) {
		// �{�[������
		output.Pos = Pos;
		output.Norm = Norm;
	} else {
		// �{�[��0
		uint uBone = input.Bone.x;
		float fWeight = input.Weight.x;
		matrix m = g_BoneWorld[uBone];
		output.Pos = fWeight * mul(Pos, m);
		output.Norm = fWeight * mul(Norm, (float3x3)m);
		// �{�[��1
		uBone = input.Bone.y;
		fWeight = input.Weight.y;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
		// �{�[��2
		uBone = input.Bone.z;
		fWeight = input.Weight.z;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
		// �{�[��3
		uBone = input.Bone.w;
		fWeight = input.Weight.w;
		m = g_BoneWorld[uBone];
		output.Pos += fWeight * mul(Pos, m);
		output.Norm += fWeight * mul(Norm, (float3x3)m);
	}
	return output;
}

//
// ���_�V�F�[�_
//
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	SKIN vSkinned = SkinVert(input);
	output.Pos = mul(vSkinned.Pos, mtxWVP);
	output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), mtxTexture).xy;
	output.Normal = mul(vSkinned.Norm, (float3x3)mtxWorld);
	output.PosForPS = mul(vSkinned.Pos, mtxWorld).xyz;

	//-- �ڃx�N�g���v�Z --
	float3 N = normalize(input.Normal);
	float3 T = normalize(input.Tangent);
	float3 B = normalize(cross(N, T));

	float3x3 invTexToWorld =
		float3x3(
			T.x, B.x, N.x,
			T.y, B.y, N.y,
			T.z, B.z, N.z
			);
	//���C�g���e�N�X�`����Ԃɕϊ�
	float3 L = normalize(vLightDir.xyz);
	output.TexSpaceLight = mul(L, invTexToWorld);


	return output;
}
