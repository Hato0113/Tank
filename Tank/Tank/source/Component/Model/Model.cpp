//------------------------------
//	モデルコンポーネント
//-- author --
//	HatoriMasashi
//------------------------------

//-- include --
#include "Model.h"
#include "Shader\ShaderManager.h"

Model::Model()
{
	m_pModel = nullptr;
	IsCameraLightSet = false;
	m_Scale = { 1.0f,1.0f,1.0f };
	m_RelativePos = { 0.0f,0.0f,0.0f };

	m_VertexShaderName = "VS_Assimp";
	m_PixelShaderName = "PS_Assimp";

	m_useLightFlag = true;

	Component::SetLayer(Layer::Back3D);
}

Model::~Model()
{
	//none
}

void Model::Update()
{
	if (!IsCameraLightSet && m_pModel)
	{
		IsCameraLightSet = true;
		Object* CamObj = parent->GetScene()->manager->FindObjectWithName("MainCamera");
		if (CamObj)
		{
			Camera* pCamera = CamObj->GetComponent<Camera>();
			if (pCamera)
				m_pModel->SetCamera(pCamera);
			else
				IsCameraLightSet = false;
		}
		else IsCameraLightSet = false;
		Object* LightObj = parent->GetScene()->manager->FindObjectWithName("Light");
		if (LightObj)
		{
			DirectionalLight* pLight = LightObj->GetComponent<DirectionalLight>();
			if (pLight)
			{
				m_pModel->SetLight(pLight);
				m_pLight = pLight;
			}
			else IsCameraLightSet = false;
		}
		else IsCameraLightSet = false;
	}
}

void Model::Draw()
{
	if (!IsCameraLightSet) return;	//カメラ / ライトが設定されていなければスキップ
	ID3D11DeviceContext* pDC = DX::DirectXManager::GetInstance().GetDeviceContext();
	using namespace DirectX;
	//-- ワールドマトリクス設定 --
	XMMATRIX mWorld = XMMatrixIdentity();
	XMFLOAT3 pos = parent->transform->GetPos();
	XMFLOAT3 size = parent->transform->GetScale();
	XMMATRIX mRot = parent->transform->GetEulerAngle().GetRotateMatrix();
	XMMATRIX mTrans = XMMatrixTranslation(pos.x + m_RelativePos.x , pos.y + m_RelativePos.y, pos.z + m_RelativePos.z);
	XMMATRIX mScale = XMMatrixScaling(size.x * m_Scale.x, size.y * m_Scale.y, size.z * m_Scale.z);
	mWorld = XMMatrixMultiply(mWorld, mScale);
	mWorld = XMMatrixMultiply(mWorld, mRot);
	mWorld = XMMatrixMultiply(mWorld, mTrans);

	//-- シェーダー設定 --
	ShaderManager::GetInstance().BindVS(m_VertexShaderName);
	ShaderManager::GetInstance().BindPS(m_PixelShaderName);

	ShaderManager::GetInstance().TextureResourceBind();

	if (!m_useLightFlag)
		m_pLight->SetEnable(false);

	XMFLOAT4X4 WorldMat;
	XMStoreFloat4x4(&WorldMat, mWorld);

	m_pModel->Draw(pDC, WorldMat, EByOpacity::eTransparentOnly);
	m_pModel->Draw(pDC, WorldMat, EByOpacity::eOpacityOnly);

	m_pLight->SetEnable(true);
}

void Model::SetScale(DirectX::XMFLOAT3 scale)
{
	m_Scale = scale;
}
void Model::SetScale(float scale)
{
	m_Scale = { scale,scale,scale };
}