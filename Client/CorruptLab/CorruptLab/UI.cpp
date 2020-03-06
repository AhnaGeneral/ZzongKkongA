
//-----------------------------------------------------------------------------
// File: UI.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "UI.h"

CUI::CUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth, float fxPosition, float fyPosition, float fzPosition)
{
	float width = FRAME_BUFFER_WIDTH / 2.f;
	float height = FRAME_BUFFER_HEIGHT / 2.f;
	CMesh* pMesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 200 , 200, 0);
	SetMesh(pMesh);
	Set2DPosition(width - 100, height - 100);
}

void CUI::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
	
	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);

	if (m_pSibling) m_pSibling->Render(pd3dCommandList, pCamera);
	if (m_pChild) m_pChild->Render(pd3dCommandList, pCamera);
}

void CUI::Set2DPosition(float x, float y)
{
	m_xmf4x4Transform._41 = x;
	m_xmf4x4Transform._42 = y;

	UpdateTransform(NULL);
}
