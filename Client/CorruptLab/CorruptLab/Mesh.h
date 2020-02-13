//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------
#pragma once

// m_ntype�� ����
#define VERTEXT_POSITION			0x01
#define VERTEXT_COLOR				0x02
#define VERTEXT_NORMAL				0x04

typedef struct MESHINFO
{
	ID3D12Resource            *m_pd3dVertexBuffer = NULL;
	ID3D12Resource            *m_pd3dVertexUploadBuffer = NULL;
				              
	ID3D12Resource            *m_pd3dIndexBuffer = NULL;
	ID3D12Resource            *m_pd3dIndexUploadBuffer = NULL;

	D3D12_VERTEX_BUFFER_VIEW  m_d3dVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW	  m_d3dIndexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY  m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT					  m_nSlot = 0;
	UINT					  m_nVertices = 0;
	UINT					  m_nStride = 0;
	UINT					  m_nOffset = 0;

	UINT					  m_nIndices = 0;
	UINT					  m_nStartIndex = 0;
	UINT                      m_nType = 0;
	int						  m_nBaseVertex = 0;
}MeshInfo;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMesh
{
public:
	CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CMesh();

private:
	int	m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void ReleaseUploadBuffers();

protected:
	MeshInfo   m_MeshInfo;
public:
	UINT GetType() { return(m_MeshInfo.m_nType); }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet) { }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMeshLoadInfo 
{
public:
	CMeshLoadInfo() { }
	~CMeshLoadInfo();

public:
	char                     m_pstrMeshName[256] = { 0 };
				             
	UINT		             m_nType = 0x00;
				             
	XMFLOAT3	             m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3	             m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);
				             
	int			             m_nVertices = 0;
	XMFLOAT3                 *m_pxmf3Positions = NULL;
	XMFLOAT4                 *m_pxmf4Colors = NULL;
	XMFLOAT3                 *m_pxmf3Normals = NULL;
	XMFLOAT2				 *m_pxmf2TexCoords = NULL;

	int						 m_nIndices = 0;
	UINT                     *m_pnIndices = NULL;

	int						 m_nSubMeshes = 0;
	int                      *m_pnSubSetIndices = NULL;
	UINT                     **m_ppnSubSetIndices = NULL;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMeshFromFile : public CMesh
{
public:
	CMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CMeshLoadInfo* pMeshInfo);
	virtual ~CMeshFromFile();

public:
	virtual void ReleaseUploadBuffers();

protected:
	ID3D12Resource            *m_pd3dPositionBuffer = NULL;
	ID3D12Resource            *m_pd3dPositionUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW  m_d3dPositionBufferView;

	ID3D12Resource* m_pd3dTexCoordBuffer = NULL;
	ID3D12Resource* m_pd3dTexCoordUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW  m_d3dTexCoordBufferView;


	int						  m_nSubMeshes = 0;
	int                       *m_pnSubSetIndices = NULL;

	ID3D12Resource            **m_ppd3dSubSetIndexBuffers = NULL;
	ID3D12Resource            **m_ppd3dSubSetIndexUploadBuffers = NULL;
	D3D12_INDEX_BUFFER_VIEW   *m_pd3dSubSetIndexBufferViews = NULL;

public:
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMeshIlluminatedFromFile : public CMeshFromFile
{
public:
	CMeshIlluminatedFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CMeshLoadInfo* pMeshInfo);
	virtual ~CMeshIlluminatedFromFile();

	virtual void ReleaseUploadBuffers();

protected:
	ID3D12Resource             *m_pd3dNormalBuffer = NULL;
	ID3D12Resource             *m_pd3dNormalUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW   m_d3dNormalBufferView;

public:
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);
};
