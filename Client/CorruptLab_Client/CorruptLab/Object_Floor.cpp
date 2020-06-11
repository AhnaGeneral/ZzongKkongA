#include "Object_Floor.h"
#include "Shader_Floor.h"

CFloor::CFloor(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CTriangleRect* pNoise = new CTriangleRect(pd3dDevice, pd3dCommandList, 500.0f, 500.0f, 1.0f, 1.0f);
	SetMesh(pNoise);

	CShader* pShader = new CFloorShader();
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, FINAL_MRT_COUNT);
	SetShader(pShader);
	Rotate(90, 0, 0);
}

CFloor::~CFloor()
{
}

void CFloor::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
	if (m_ppMaterials[0]->m_pShader)
		m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera, nPipelineState);

	if (nPipelineState == 0)
		m_ppMaterials[0]->UpdateShaderVariable(pd3dCommandList);

	m_pMesh->Render(pd3dCommandList,0);
}
