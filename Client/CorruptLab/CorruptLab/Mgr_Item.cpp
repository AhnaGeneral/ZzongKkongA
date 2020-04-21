#include "Mgr_Item.h"
#include "Object_RotatingItem.h"

CItemMgr* CItemMgr::m_pInstance = NULL;

void CItemMgr::Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_pRotatingItem = new CRotatingItem(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}
