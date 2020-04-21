#include "Object_RotatingItem.h"

CRotatingItem::CRotatingItem(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	for (int i = 0; i < 3; i++)
		m_pItemTextures[i] = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	m_pItemTextures[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Inventory/HandLight.dds", 0);
	m_pItemTextures[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Inventory/HP_Kit.dds", 0);
	m_pItemTextures[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Inventory/Pill.dds", 0);
}

