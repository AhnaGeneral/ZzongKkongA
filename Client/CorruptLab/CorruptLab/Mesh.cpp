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

	//[È®ÀÎ(X)]
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
