#include "stdafx.h"
#include "UIOBJ.h"
#include "Shader.h"

CUIOBJ::CUIOBJ(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{

}

CUIOBJ::~CUIOBJ()
{
}

void CUIOBJ::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	OnPrepareRender();
	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);

	//if (m_pSibling) m_pSibling->Render(pd3dCommandList, pCamera);
	//if (m_pChild) m_pChild->Render(pd3dCommandList, pCamera);
}
