#include "stdafx.h"
#include "Object_Terrain.h"
#include "Shader_Terrain.h"
// Terrain ==================================================================================
CHeightMapTerrain::CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{

	m_nWidth = nWidth; // 257 
	m_nLength = nLength; // 257

	int cxQuadsPerBlock = nBlockWidth - 1; //8
	int czQuadsPerBlock = nBlockLength - 1; //8

	m_xmf3Scale = xmf3Scale;

	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock; //256 / 8 = 32
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	m_nMeshes = cxBlocks * czBlocks; // 32*32 = 1024
	m_ppMeshes = new CMesh * [m_nMeshes]; // 1024? 
	for (int i = 0; i < m_nMeshes; i++)	
		m_ppMeshes[i] = NULL;

	CHeightMapGridMesh* pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++) // z:0 ~ z:31
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++) //x:0 ~ x:31
		{
			xStart = x * (nBlockWidth - 1); // x * 8  :: 0 , 8 , 16, 24 .... 248 
			zStart = z * (nBlockLength - 1); // z * 8
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, pd3dCommandList, xStart, zStart,
				nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			SetMesh(x + (z * cxBlocks), pHeightMapGridMesh);
			//std:: d
		}
	}

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CTexture* pTerrainTexture = new CTexture(2, RESOURCE_TEXTURE2D, 0);

	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Terrain_Base.dds", 0);
	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Terrain_Detail.dds", 1);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256ÀÇ ¹è¼ö
	CTerrainShader* pTerrainShader = new CTerrainShader();
	pTerrainShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature,3);
	pTerrainShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pTerrainShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 2);
	pTerrainShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTerrainTexture, ROOT_PARAMETER_TERRAIN_TEX, true);
	pTerrainShader->CreateConstantBufferViews(pd3dDevice, pd3dCommandList, 1, m_pd3dcbGameObjects, ncbElementBytes);

	SetShader(pTerrainShader);

	m_ppMaterials[0]->SetTexture(pTerrainTexture);

	SetCbvGPUDescriptorHandle(pTerrainShader->GetGPUCbvDescriptorStartHandle());
}

CHeightMapTerrain::~CHeightMapTerrain(void)
{
	if (m_pHeightMapImage)
		delete m_pHeightMapImage;
	if (m_nMeshes > 0)
	{
		for (int i = 0; i < m_nMeshes; ++i)
			m_ppMeshes[i]->Release();
	}
}

void CHeightMapTerrain::SetMesh(int nIndex, CMesh* pMesh)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}
}

void CHeightMapTerrain::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CHeightMapTerrain::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	
}


void CHeightMapTerrain::ChangePipeLine()
{
	m_bPipelineStateIndex = (!m_bPipelineStateIndex);
}

void CHeightMapTerrain::SetPipelinemode()
{
	m_bPipelineStateIndex = (!m_bPipelineStateIndex);
}

void CHeightMapTerrain::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	OnPrepareRender();

	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera, m_bPipelineStateIndex);
	m_ppMaterials[0]->UpdateShaderVariable(pd3dCommandList);
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
	UpdateShaderVariables(pd3dCommandList);

	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Render(pd3dCommandList, i);
		}
	}
}

void CHeightMapTerrain::ReleaseUploadBuffers()
{
	if (m_nMeshes)
		for (int i = 0; i < m_nMeshes; ++i)
			m_ppMeshes[i]->ReleaseUploadBuffers();
}

