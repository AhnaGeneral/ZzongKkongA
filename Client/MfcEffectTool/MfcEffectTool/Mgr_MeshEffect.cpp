#include "Mgr_MeshEffect.h"
#include "Object.h"
CMgr_EffectMesh* CMgr_EffectMesh::m_pInstance = nullptr;

void CMgr_EffectMesh::Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_pMeshEffect = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList,
		pd3dGraphicsRootSignature, "Model/Sword.bin", NULL, false);
	//m_pEffectTransform = new EffectTransform; 

	m_pMeshEffect->SetPosition(m_EffectTransform.xmf3Position);
}

XMFLOAT3* CMgr_EffectMesh::GetRotationPoint()
{
	return &(m_EffectTransform.xmf3Rotation);
}

XMFLOAT3* CMgr_EffectMesh::GetPositionPoint()
{
	return &(m_EffectTransform.xmf3Position);
}

XMFLOAT3* CMgr_EffectMesh::GetSizePoint()
{
	return &(m_EffectTransform.xmf3Size);
}

void CMgr_EffectMesh::SetPosition()
{
	m_pMeshEffect->SetPosition(m_EffectTransform.xmf3Position);
}

void CMgr_EffectMesh::SetSize()
{
	m_pMeshEffect->SetScale(m_EffectTransform.xmf3Size.x,
		m_EffectTransform.xmf3Size.y, m_EffectTransform.xmf3Size.z);
}

void CMgr_EffectMesh::SetRotation()
{
	m_pMeshEffect->Rotate(m_EffectTransform.xmf3Rotation.x,
		m_EffectTransform.xmf3Rotation.y, m_EffectTransform.xmf3Rotation.z);
}


void CMgr_EffectMesh::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	if (m_pMeshEffect)m_pMeshEffect->Render(pd3dCommandList, pCamera);
}