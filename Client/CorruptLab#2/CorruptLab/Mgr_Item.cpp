#include "Mgr_Item.h"
#include "Object_RotatingItem.h"

CItemMgr* CItemMgr::m_pInstance = NULL;

void CItemMgr::Initialize(ID3D12Device* pd3dDevice, 
	    ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_pRotatingItem = new CRotatingItem(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

void CItemMgr::BillboardUIRender(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	m_pRotatingItem->Render(pd3dCommandList, pCamera);
}

void CItemMgr::Update(float fElapsedTime)
{
	m_pRotatingItem->Animate(fElapsedTime, NULL);
}

void CItemMgr::GetItem(int iType, XMFLOAT3 Pos)
{
	Pos.y += 2.f;
	m_pRotatingItem->m_nItemType = iType;
	m_pRotatingItem->SetPosition(Pos);
	m_pRotatingItem->m_bAnimate = true;
}
