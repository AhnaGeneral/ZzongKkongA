#include "stdafx.h"
#include "Shader_BillBoard.h"
#include "Shader_Noise.h"
#include "Object_BillBoard.h"
#include "Mesh_SkyBox.h"

// skybox ==================================================================================================
CSkyBox::CSkyBox(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature) : CGameObject()
{
	CSkyBoxMesh* pSkyBoxMesh = new CSkyBoxMesh(pd3dDevice, pd3dCommandList, 20.0f, 20.0f, 2.0f);
	SetMesh(pSkyBoxMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CTexture* pSkyBoxTexture = new CTexture(1, RESOURCE_TEXTURE_CUBE, 0);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/TestSkyBox.dds", 0);

	CSkyBoxShader* pSkyBoxShader = new CSkyBoxShader();
	pSkyBoxShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 3);
	pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pSkyBoxShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 1);
	pSkyBoxShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pSkyBoxTexture, ROOT_PARAMETER_SKYBOX_TEX, false);

	SetShader(pSkyBoxShader);
	m_ppMaterials[0]->SetTexture(pSkyBoxTexture);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	XMFLOAT3 xmf3CameraPos = pCamera->GetPosition();
	SetPosition(xmf3CameraPos.x, xmf3CameraPos.y, xmf3CameraPos.z);

	CGameObject::Render(pd3dCommandList, pCamera);

}

CBillboard::CBillboard ( ID3D12Device* pd3dDevice,
	                     ID3D12GraphicsCommandList* pd3dCommandList,
	                     float fWidth, float fHeight, float fDepth, 
	                     float fPosX, float fPosY, float fPosZ )
{
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

CBillboard::~CBillboard()
{
}

void CBillboard::Animates(float fTimeElapsed, CCamera* pCamera)
{
	XMFLOAT3 xmf3UpdateCameraPosition = pCamera->GetPosition();
	SetLookAt(xmf3UpdateCameraPosition);
}

void CBillboard::SetLookAt(XMFLOAT3& xmf3TargetCamera)
{
	XMFLOAT3 xmf3UP(0.0f, 1.0f, 0.0f);
	XMFLOAT3 xmf3Position(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	XMFLOAT3 xmf3Look = Vector3::Subtract(xmf3TargetCamera, xmf3Position);
	XMFLOAT3 xmf3Right = Vector3::CrossProduct(xmf3UP, xmf3Look);
	m_xmf4x4World._11 = xmf3Right.x; m_xmf4x4World._12 = xmf3Right.y; m_xmf4x4World._13 = xmf3Right.z;
	m_xmf4x4World._21 = xmf3UP.x; m_xmf4x4World._22 = xmf3UP.y; m_xmf4x4World._23 = xmf3UP.z;
	m_xmf4x4World._31 = xmf3Look.x; m_xmf4x4World._32 = xmf3Look.y; m_xmf4x4World._33 = xmf3Look.y;
}

CObjectNosie::CObjectNosie(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CTriangleRect* pNoise = new CTriangleRect(pd3dDevice, pd3dCommandList, 100.0f, 100.0f, 1.0f, 1.0f);
	SetMesh(pNoise);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CTexture* pNoiseTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Noise/fire01.dds", 0);
	//pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Noise/alpha01.dds", 1);
	//pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Noise/noise01.dds", 2);

	CSkyBoxShader* pNosieShader = new CSkyBoxShader();
	pNosieShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 3);
	pNosieShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pNosieShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 1);
	pNosieShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pNoiseTexture, ROOT_PARAMETER_NOISE_TEX, false);
	SetShader(pNosieShader);
	m_ppMaterials[0]->SetTexture(pNoiseTexture);
}

CObjectNosie::~CObjectNosie()
{
}

void CObjectNosie::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera, 0);
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}
