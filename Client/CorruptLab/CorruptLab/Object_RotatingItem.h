#pragma once
#include "Object_UI.h"
class CRotatingItem :
	public CMRTUI
{
public:

	CRotatingItem(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);

public:
	bool	m_bAnimate = false;
	float	m_fElapsedTIme = 0.0f;
	int		m_nItemType = 0;
	CTexture* m_pItemTextures[3];

};

