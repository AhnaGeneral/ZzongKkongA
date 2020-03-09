#include "stdafx.h"
#include "Object_UIOBJ.h"
#include "Shader.h"

CUIOBJ::CUIOBJ(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nobjectID = 0; 
}

CUIOBJ::~CUIOBJ()
{
}

void CUIOBJ::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);
	UINT getobjectID = m_nobjectID;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 4, &getobjectID, 16);

}

void CUIOBJ::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	OnPrepareRender();
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CUIOBJ::SetObjectID(UINT objectID)
{
	m_nobjectID = objectID; 
}

void CUIOBJ::Set2DPosition(float x, float y)
{
	m_xmf4x4Transform._41 = x;
	m_xmf4x4Transform._42 = y;

	UpdateTransform(NULL);
}
