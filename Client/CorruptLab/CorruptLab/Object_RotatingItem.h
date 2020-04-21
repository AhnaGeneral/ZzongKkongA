#pragma once
#include "Object_UI.h"
class CRotatingItem :
	public CMRTUI
{
public:

	CRotatingItem(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);

	int		m_nItemType = 0;
	CTexture* m_pItemTextures[3];

};

