//-----------------------------------------------------------------------------
// File: CMesh.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"

CMesh::CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }

CMesh::~CMesh()
{
	if (m_MeshInfo.m_pd3dVertexBuffer) 
		m_MeshInfo.m_pd3dVertexBuffer->Release();

	if (m_MeshInfo.m_pd3dIndexBuffer) 
		m_MeshInfo.m_pd3dIndexBuffer->Release();

	if (m_MeshInfo.m_pd3dVertexUploadBuffer)
		m_MeshInfo.m_pd3dVertexUploadBuffer->Release();

	if (m_MeshInfo.m_pd3dIndexUploadBuffer) 
		m_MeshInfo.m_pd3dIndexUploadBuffer->Release();
}

void CMesh::ReleaseUploadBuffers()
{
	if (m_MeshInfo.m_pd3dVertexUploadBuffer) 
		m_MeshInfo.m_pd3dVertexUploadBuffer->Release();
	if (m_MeshInfo.m_pd3dIndexUploadBuffer) 
		m_MeshInfo.m_pd3dIndexUploadBuffer->Release();
	m_MeshInfo.m_pd3dVertexUploadBuffer = NULL;
	m_MeshInfo.m_pd3dIndexUploadBuffer = NULL;
};

void CMesh::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->IASetPrimitiveTopology(m_MeshInfo.m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_MeshInfo.m_nSlot, 1, &m_MeshInfo.m_d3dVertexBufferView);
	if (m_MeshInfo.m_pd3dIndexBuffer)
	{
		pd3dCommandList->IASetIndexBuffer(&m_MeshInfo.m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_MeshInfo.m_nIndices, 1, 0, 0, 0);
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_MeshInfo.m_nVertices, 1, m_MeshInfo.m_nOffset, 0);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

CStandardMesh::CStandardMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, MeshLoadInfo* pMeshInfo) : CMesh(pd3dDevice,  pd3dCommandList)
{
	m_MeshInfo.m_nVertices = pMeshInfo->m_nVertices;
	m_MeshInfo.m_nType = pMeshInfo->m_nType;

	CStandardVertex *pVerticse = new CStandardVertex[m_MeshInfo.m_nVertices];
	for (int i = 0; i < m_MeshInfo.m_nVertices; ++i)
	{
		pVerticse[i].m_xmf3Position = pMeshInfo->m_pxmf3Positions[i];
		pVerticse[i].m_xmf3Normal   = pMeshInfo->m_pxmf3Normals[i];
		pVerticse[i].m_xmf2TexCoord = pMeshInfo->m_pxmf2TexCoords[i];
	}
	m_MeshInfo.m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVerticse, sizeof(CStandardVertex) * m_MeshInfo.m_nVertices,
		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_MeshInfo.m_pd3dVertexUploadBuffer);

	m_MeshInfo.m_d3dVertexBufferView.BufferLocation = m_MeshInfo.m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_MeshInfo.m_d3dVertexBufferView.StrideInBytes = sizeof(CStandardVertex);
	m_MeshInfo.m_d3dVertexBufferView.SizeInBytes = sizeof(CStandardVertex) * m_MeshInfo.m_nVertices;

	//[확인(X)]
	m_nSubMeshes = pMeshInfo->m_nSubMeshes;

	if (m_nSubMeshes > 0)
	{
		m_ppd3dSubSetIndexBuffers = new ID3D12Resource * [m_nSubMeshes];
		m_ppd3dSubSetIndexUploadBuffers = new ID3D12Resource * [m_nSubMeshes];
		m_pd3dSubSetIndexBufferViews = new D3D12_INDEX_BUFFER_VIEW[m_nSubMeshes];

		m_pnSubSetIndices = new int[m_nSubMeshes];

		for (int i = 0; i < m_nSubMeshes; i++)
		{
			m_pnSubSetIndices[i] = pMeshInfo->m_pnSubSetIndices[i];
			m_ppd3dSubSetIndexBuffers[i] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pMeshInfo->m_ppnSubSetIndices[i], sizeof(UINT) * m_pnSubSetIndices[i], 
				D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_ppd3dSubSetIndexUploadBuffers[i]);

			m_pd3dSubSetIndexBufferViews[i].BufferLocation = m_ppd3dSubSetIndexBuffers[i]->GetGPUVirtualAddress();
			m_pd3dSubSetIndexBufferViews[i].Format = DXGI_FORMAT_R32_UINT;
			m_pd3dSubSetIndexBufferViews[i].SizeInBytes = sizeof(UINT) * pMeshInfo->m_pnSubSetIndices[i];
		}
	}
}

CStandardMesh::~CStandardMesh()
{	
}
void CStandardMesh::ReleaseUploadBuffers()
{
	CMesh::ReleaseUploadBuffers();

	if ((m_nSubMeshes > 0) && m_ppd3dSubSetIndexUploadBuffers)
	{
		for (int i = 0; i < m_nSubMeshes; i++)
		{
			if (m_ppd3dSubSetIndexUploadBuffers[i]) m_ppd3dSubSetIndexUploadBuffers[i]->Release();
		}
		if (m_ppd3dSubSetIndexUploadBuffers) delete[] m_ppd3dSubSetIndexUploadBuffers;
		m_ppd3dSubSetIndexUploadBuffers = NULL;
	}
}

void CStandardMesh::Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet)
{
	pd3dCommandList->IASetPrimitiveTopology(m_MeshInfo.m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_MeshInfo.m_nSlot, 1, &m_MeshInfo.m_d3dVertexBufferView);

	if ((m_nSubMeshes > 0) && (nSubSet < m_nSubMeshes))
	{
		pd3dCommandList->IASetIndexBuffer(&(m_pd3dSubSetIndexBufferViews[nSubSet]));
		pd3dCommandList->DrawIndexedInstanced(m_pnSubSetIndices[nSubSet], 1, 0, 0, 0);
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_MeshInfo.m_nVertices, 1, m_MeshInfo.m_nOffset, 0);
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////////////
////
//CSkinnedMesh::CSkinnedMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) : CStandardMesh(pd3dDevice, pd3dCommandList)
//{
//}
//
//CSkinnedMesh::~CSkinnedMesh()
//{
//	if (m_pxmn4BoneIndices) delete[] m_pxmn4BoneIndices;
//	if (m_pxmf4BoneWeights) delete[] m_pxmf4BoneWeights;
//
//	if (m_ppSkinningBoneFrameCaches) delete[] m_ppSkinningBoneFrameCaches;
//	if (m_ppstrSkinningBoneNames) delete[] m_ppstrSkinningBoneNames;
//
//	if (m_pxmf4x4BindPoseBoneOffsets) delete[] m_pxmf4x4BindPoseBoneOffsets;
//	if (m_pd3dcbBindPoseBoneOffsets) m_pd3dcbBindPoseBoneOffsets->Release();
//
//	if (m_pd3dBoneIndexBuffer) m_pd3dBoneIndexBuffer->Release();
//	if (m_pd3dBoneWeightBuffer) m_pd3dBoneWeightBuffer->Release();
//
//	ReleaseShaderVariables();
//}
//
//void CSkinnedMesh::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
//{
//}
//
//void CSkinnedMesh::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
//{
//	if (m_pd3dcbBindPoseBoneOffsets)
//	{
//		D3D12_GPU_VIRTUAL_ADDRESS d3dcbBoneOffsetsGpuVirtualAddress = m_pd3dcbBindPoseBoneOffsets->GetGPUVirtualAddress();
//		pd3dCommandList->SetGraphicsRootConstantBufferView(11, d3dcbBoneOffsetsGpuVirtualAddress); //Skinned Bone Offsets
//	}
//
//	if (m_pd3dcbSkinningBoneTransforms)
//	{
//		D3D12_GPU_VIRTUAL_ADDRESS d3dcbBoneTransformsGpuVirtualAddress = m_pd3dcbSkinningBoneTransforms->GetGPUVirtualAddress();
//		pd3dCommandList->SetGraphicsRootConstantBufferView(12, d3dcbBoneTransformsGpuVirtualAddress); //Skinned Bone Transforms
//
//		for (int j = 0; j < m_nSkinningBones; j++)
//		{
//			XMStoreFloat4x4(&m_pcbxmf4x4MappedSkinningBoneTransforms[j], XMMatrixTranspose(XMLoadFloat4x4(&m_ppSkinningBoneFrameCaches[j]->m_xmf4x4World)));
//		}
//	}
//}
//
//void CSkinnedMesh::ReleaseShaderVariables()
//{
//}
//
//void CSkinnedMesh::ReleaseUploadBuffers()
//{
//	CStandardMesh::ReleaseUploadBuffers();
//
//	if (m_pd3dBoneIndexUploadBuffer) m_pd3dBoneIndexUploadBuffer->Release();
//	m_pd3dBoneIndexUploadBuffer = NULL;
//
//	if (m_pd3dBoneWeightUploadBuffer) m_pd3dBoneWeightUploadBuffer->Release();
//	m_pd3dBoneWeightUploadBuffer = NULL;
//}
//
//void CSkinnedMesh::PrepareSkinning(CGameObject* pModelRootObject)
//{
//	m_ppSkinningBoneFrameCaches = new CGameObject * [m_nSkinningBones];
//	for (int j = 0; j < m_nSkinningBones; j++)
//	{
//		m_ppSkinningBoneFrameCaches[j] = pModelRootObject->FindFrame(m_ppstrSkinningBoneNames[j]);
//	}
//}
//
//void CSkinnedMesh::LoadSkinInfoFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile)
//{
//	char pstrToken[64] = { '\0' };
//	UINT nReads = 0;
//
//	::ReadStringFromFile(pInFile, m_pstrMeshName);
//
//	for (; ; )
//	{
//		::ReadStringFromFile(pInFile, pstrToken);
//		if (!strcmp(pstrToken, "<BonesPerVertex>:"))
//		{
//			m_nBonesPerVertex = ::ReadIntegerFromFile(pInFile);
//		}
//		else if (!strcmp(pstrToken, "<Bounds>:"))
//		{
//			nReads = (UINT)::fread(&m_xmf3AABBCenter, sizeof(XMFLOAT3), 1, pInFile);
//			nReads = (UINT)::fread(&m_xmf3AABBExtents, sizeof(XMFLOAT3), 1, pInFile);
//		}
//		else if (!strcmp(pstrToken, "<BoneNames>:"))
//		{
//			m_nSkinningBones = ::ReadIntegerFromFile(pInFile);
//			if (m_nSkinningBones > 0)
//			{
//				m_ppstrSkinningBoneNames = new char[m_nSkinningBones][64];
//				for (int i = 0; i < m_nSkinningBones; i++) ::ReadStringFromFile(pInFile, m_ppstrSkinningBoneNames[i]);
//			}
//		}
//		else if (!strcmp(pstrToken, "<BoneOffsets>:"))
//		{
//			m_nSkinningBones = ::ReadIntegerFromFile(pInFile);
//			if (m_nSkinningBones > 0)
//			{
//				m_pxmf4x4BindPoseBoneOffsets = new XMFLOAT4X4[m_nSkinningBones];
//				nReads = (UINT)::fread(m_pxmf4x4BindPoseBoneOffsets, sizeof(XMFLOAT4X4), m_nSkinningBones, pInFile);
//
//				UINT ncbElementBytes = (((sizeof(XMFLOAT4X4) * SKINNED_ANIMATION_BONES) + 255) & ~255); //256의 배수
//				m_pd3dcbBindPoseBoneOffsets = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//				m_pd3dcbBindPoseBoneOffsets->Map(0, NULL, (void**)&m_pcbxmf4x4MappedBindPoseBoneOffsets);
//
//				for (int i = 0; i < m_nSkinningBones; i++)
//				{
//					XMStoreFloat4x4(&m_pcbxmf4x4MappedBindPoseBoneOffsets[i], XMMatrixTranspose(XMLoadFloat4x4(&m_pxmf4x4BindPoseBoneOffsets[i])));
//				}
//			}
//		}
//		else if (!strcmp(pstrToken, "<BoneIndices>:"))
//		{
//			m_nType |= VERTEXT_BONE_INDEX_WEIGHT;
//
//			m_nVertices = ::ReadIntegerFromFile(pInFile);
//			if (m_nVertices > 0)
//			{
//				m_pxmn4BoneIndices = new XMINT4[m_nVertices];
//
//				nReads = (UINT)::fread(m_pxmn4BoneIndices, sizeof(XMINT4), m_nVertices, pInFile);
//				m_pd3dBoneIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmn4BoneIndices, sizeof(XMINT4) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dBoneIndexUploadBuffer);
//
//				m_d3dBoneIndexBufferView.BufferLocation = m_pd3dBoneIndexBuffer->GetGPUVirtualAddress();
//				m_d3dBoneIndexBufferView.StrideInBytes = sizeof(XMINT4);
//				m_d3dBoneIndexBufferView.SizeInBytes = sizeof(XMINT4) * m_nVertices;
//			}
//		}
//		else if (!strcmp(pstrToken, "<BoneWeights>:"))
//		{
//			m_nType |= VERTEXT_BONE_INDEX_WEIGHT;
//
//			m_nVertices = ::ReadIntegerFromFile(pInFile);
//			if (m_nVertices > 0)
//			{
//				m_pxmf4BoneWeights = new XMFLOAT4[m_nVertices];
//
//				nReads = (UINT)::fread(m_pxmf4BoneWeights, sizeof(XMFLOAT4), m_nVertices, pInFile);
//				m_pd3dBoneWeightBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf4BoneWeights, sizeof(XMFLOAT4) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dBoneWeightUploadBuffer);
//
//				m_d3dBoneWeightBufferView.BufferLocation = m_pd3dBoneWeightBuffer->GetGPUVirtualAddress();
//				m_d3dBoneWeightBufferView.StrideInBytes = sizeof(XMFLOAT4);
//				m_d3dBoneWeightBufferView.SizeInBytes = sizeof(XMFLOAT4) * m_nVertices;
//			}
//		}
//		else if (!strcmp(pstrToken, "</SkinningInfo>"))
//		{
//			break;
//		}
//	}
//}
//
//void CSkinnedMesh::OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext)
//{
//	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[7] = { m_d3dPositionBufferView, m_d3dTextureCoord0BufferView, m_d3dNormalBufferView, m_d3dTangentBufferView, m_d3dBiTangentBufferView, m_d3dBoneIndexBufferView, m_d3dBoneWeightBufferView };
//	pd3dCommandList->IASetVertexBuffers(m_nSlot, 7, pVertexBufferViews);
//}