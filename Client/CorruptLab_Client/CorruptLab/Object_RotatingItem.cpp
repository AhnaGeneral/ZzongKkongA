#include "Object_RotatingItem.h"
#include "Shader_BillboardUI.h"
#include "Mgr_Item.h"

CRotatingItem::CRotatingItem(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	for (int i = 0; i < 4; i++)
		m_pItemTextures[i] = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	m_pItemTextures[ITEM_TYPE_HANDLIGHT]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Inventory/HandLight.dds", 0);
	m_pItemTextures[ITEM_TYPE_HPKIT]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Inventory/HP_Kit.dds", 0);
	m_pItemTextures[ITEM_TYPE_PILL]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Inventory/Pill.dds", 0);
	m_pItemTextures[ITEM_TYPE_MAPSEGMENT]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Inventory/MapSegment.dds", 0);

	CTriangleRect* pMesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 3, 3, 0, 1.f);
	SetMesh(pMesh);

	CBillboardUIShader* pShader = new CBillboardUIShader();
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, FINAL_MRT_COUNT);
	pShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 4);
	pShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pItemTextures[ITEM_TYPE_HANDLIGHT], ROOT_PARAMETER_ALBEDO_TEX,0);
	pShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pItemTextures[ITEM_TYPE_HPKIT], ROOT_PARAMETER_ALBEDO_TEX, 0);
	pShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pItemTextures[ITEM_TYPE_PILL], ROOT_PARAMETER_ALBEDO_TEX, 0);
	pShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pItemTextures[ITEM_TYPE_MAPSEGMENT], ROOT_PARAMETER_ALBEDO_TEX, 0);

	SetShader(pShader);

	//SetPosition(XMFLOAT3(464.0f, 40.0f, 354.0f));
}

void CRotatingItem::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	if (!m_bAnimate) return;

	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera);
	
	m_pItemTextures[m_nItemType]->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

//void CRotatingItem::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
//{
//	//m_ItemReact.m_fTIme -= 0.01f;
//	//if (m_ItemReact.m_fTIme < 0.1f)
//	//{
//	//	m_ItemReact.m_fTIme = .0f;
//	//}
//
//	XMFLOAT4X4 xmf4x4World;
//	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
//	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);
//
//	UINT getobjectID = m_nobjectID;
//	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 4, &getobjectID, 16);
//
//	SetItemCount(CItemMgr::GetInstance()->GetItemNums());
//	//cout << "1:	" << m_ItemReact.m_f4iTemCount.x << endl; 
//	//cout << "2:	" << m_ItemReact.m_f4iTemCount.y << endl;
//	//cout << "3:	" << m_ItemReact.m_f4iTemCount.z << endl;
//	//cout << "4:	" << m_ItemReact.m_f4iTemCount.w << endl;
//	memcpy(m_pcbItemReact, &m_ItemReact, sizeof(CB_ITEM));
//	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbItemReact->GetGPUVirtualAddress();
//	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_ITEM_REACT, d3dGpuVirtualAddress);
//}

CRotatingItem::~CRotatingItem()
{
	//if (m_pItemTextures)
	//{
	//	for (int i = 0; i < m_nItemType; ++i)
	//	{
	//		m_pItemTextures[i]->ReleaseUploadBuffers();
	//		m_pItemTextures[i]->Release();
	//	}
	//}
	//m_ppMaterials[0]->m_pShader->ReleaseUploadBuffers();
	//m_ppMaterials[0]->m_pShader->Release(); 

}

void CRotatingItem::Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	if (!m_bAnimate) return;
	
	m_fElapsedTIme += fTimeElapsed;
	Rotate(0, fTimeElapsed * 200, 0);
	MoveUp(fTimeElapsed * 6);

	if (m_fElapsedTIme > 1.5f)
	{
		m_fElapsedTIme = 0.0f;
		m_bAnimate = false;
		CItemMgr::GetInstance()->GiveItemToPlayer(m_nItemType);
	}
	UpdateTransform(NULL);
}

