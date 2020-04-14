#include "stdafx.h"
#include "Object_UI.h"
#include "Shader.h"
#include "Shader_Minimap.h"

CUI::CUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nobjectID = 0; 
}

CUI::~CUI()
{
}

void CUI::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);
	UINT getobjectID = m_nobjectID;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 4, &getobjectID, 16);

}

void CUI::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CUI::SetObjectID(UINT objectID)
{
	m_nobjectID = objectID; 
}

void CUI::Set2DPosition(float x, float y)
{
	m_xmf4x4Transform._41 = x;
	m_xmf4x4Transform._42 = y;

	UpdateTransform(NULL);
}


CMinimap::CMinimap()
{
}

CMinimap::CMinimap(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CMinimapShader* pShader = new CMinimapShader();
	pShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 3, 1);
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 5);

	CTexture* pMinimapTex = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pMinimapTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Minimap.dds", 0);
	pShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pMinimapTex, ROOT_PARAMETER_LIGHT_MRT, 0);
	SetShader(pShader);
	m_ppMaterials[0]->SetTexture(pMinimapTex);
}

void CMinimap::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// [ 직교투영 ] --------------------------------------------------------------------------------
	UINT ncbElementBytes = ((sizeof(XMFLOAT3) + 255) & ~255); //256의 배수  
	m_pd3dcbPlayerPosition = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pd3dcbPlayerPosition->Map(0, NULL, (void**)&m_pcbPlayerPosition);
}

void CMinimap::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera);
	m_ppMaterials[0]->UpdateShaderVariable(pd3dCommandList);

	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CMinimap::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);

	memcpy(m_pcbPlayerPosition, m_PlayerPosition, sizeof(XMFLOAT3));
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbPlayerPosition->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_PLAYER_POS, d3dGpuVirtualAddress);
	
}
