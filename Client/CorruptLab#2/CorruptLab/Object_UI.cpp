#include "stdafx.h"
#include "Object_UI.h"
#include "Shader.h"
#include "Shader_Minimap.h"
#include "Shader_BaseUI.h"
#include "Shader_Item.h"
#include "shader_ObjHP.h"


CMRTUI::CMRTUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nobjectID = 0;
}

CMRTUI::~CMRTUI()
{
}

void CMRTUI::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);

	UINT getobjectID = m_nobjectID;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 4, &getobjectID, 16);

}

void CMRTUI::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CMRTUI::SetObjectID(UINT objectID)
{
	m_nobjectID = objectID;
}

void CMRTUI::Set2DPosition(float x, float y)
{
	m_xmf4x4Transform._41 = x;
	m_xmf4x4Transform._42 = y;

	UpdateTransform(NULL);
}

// [ CUI_MiniMap ] =======================================================================================================
CUI_MiniMap::CUI_MiniMap()
{
}

CUI_MiniMap::CUI_MiniMap(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
}

void CUI_MiniMap::InterLinkShaderTexture(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* Texture)
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

void CUI_MiniMap::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(XMFLOAT3) + 255) & ~255); //256의 배수  
	m_pd3dcbPlayerPosition = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pd3dcbPlayerPosition->Map(0, NULL, (void**)&m_pcbPlayerPosition);
}

void CUI_MiniMap::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera);
	m_ppMaterials[0]->UpdateShaderVariable(pd3dCommandList);

	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CUI_MiniMap::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);

	memcpy(m_pcbPlayerPosition, m_PlayerPosition, sizeof(XMFLOAT3));
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbPlayerPosition->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_PLAYER_POS, d3dGpuVirtualAddress);

}


// [ CUI_HP ] ==========================================================================================================
CUI_Root::CUI_Root()
{
}

CUI_Root::CUI_Root(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
}

void CUI_Root::InterLinkShaderTexture(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* Texture)
{
	CShader_BaseUI* pShader = new CShader_BaseUI();
	pShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 1);
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 5);

	pShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)Texture, ROOT_PARAMETER_HP_TEX, true);
	SetShader(pShader);
	m_ppMaterials[0]->SetTexture((CTexture*)Texture);
}


void CUI_Root::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void CUI_Root::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera);
	m_ppMaterials[0]->UpdateShaderVariable(pd3dCommandList);

	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CUI_Root::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);
}


// [ CUI_Inventory ] ======================================================================================================
CUI_ITem::CUI_ITem()
{

}

CUI_ITem::CUI_ITem(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
}

void CUI_ITem::InterLinkShaderTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* Texture)
{
	CShader_Item* pShader = new CShader_Item();
	pShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 3);
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 5);

	pShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)Texture, ROOT_PARAMETER_ITEM_TEX, true);
	SetShader(pShader);
	m_ppMaterials[0]->SetTexture((CTexture*)Texture);
}

void CUI_ITem::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::CreateShaderVariables(pd3dDevice, pd3dCommandList);

	UINT ncbElementBytes = ((sizeof(UINT) + 255) & ~255); //256의 배수  
	m_pd3dcbItemReact = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pd3dcbItemReact->Map(0, NULL, (void**)&m_pcbItemReact);

}

void CUI_ITem::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera);
	m_ppMaterials[0]->UpdateShaderVariable(pd3dCommandList);

	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CUI_ITem::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);

	UINT getobjectID = m_nobjectID;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 4, &getobjectID, 16);

	memcpy(m_pcbItemReact, m_ItemReact, sizeof(UINT));
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbItemReact->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_ITEM_REACT, d3dGpuVirtualAddress);

}

void CUI_HP::InterLinkShaderTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
	ID3D12RootSignature* pd3dGraphicsRootSignature, void* Texture)
{
	CShader_ObjHP* pShader = new CShader_ObjHP();
	pShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 1);
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 5);

	pShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)Texture, ROOT_PARAMETER_HP_TEX, true);
	SetShader(pShader);
	m_ppMaterials[0]->SetTexture((CTexture*)Texture);
}

void CUI_HP::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::CreateShaderVariables(pd3dDevice, pd3dCommandList);

	UINT ncbElementBytes = ((sizeof(int) + 255) & ~255); //256의 배수  
	m_pd3dcbPlayerHP = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pd3dcbPlayerHP->Map(0, NULL, (void**)&m_pcbPlayerHP);

}

void CUI_HP::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera);
	m_ppMaterials[0]->UpdateShaderVariable(pd3dCommandList);

	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CUI_HP::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);
	//std::cout << *m_PlayerHP << std::endl;

	memcpy(m_pcbPlayerHP, m_PlayerHP, sizeof(int));
	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbPlayerHP->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_PLAYER_HP, d3dGpuVirtualAddress);
}
