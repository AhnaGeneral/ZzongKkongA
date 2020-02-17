//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------
#pragma once

class CGameObject; 

// m_ntype¿ª ∞·¡§
#define VERTEXT_POSITION				0x0001
#define VERTEXT_COLOR					0x0002
#define VERTEXT_NORMAL					0x0004
#define VERTEXT_TANGENT                 0x0008 
#define VERTEXT_TEXTURE_COORD0			0x0010
#define VERTEXT_TEXTURE_COORD1			0x0020

#define VERTEXT_BONE_INDEX_WEIGHT		0x1000

#define VERTEXT_TEXTURE					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_DETAIL					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TEXTURE			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_TANGENT_TEXTURE	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_DETAIL			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TANGENT__DETAIL	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)


typedef struct MESHINFO
{
	ID3D12Resource                  *m_pd3dVertexBuffer = NULL;
	ID3D12Resource                  *m_pd3dVertexUploadBuffer = NULL;
				                    
	ID3D12Resource                  *m_pd3dIndexBuffer = NULL;
	ID3D12Resource                  *m_pd3dIndexUploadBuffer = NULL;
							      
	D3D12_VERTEX_BUFFER_VIEW        m_d3dVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW	        m_d3dIndexBufferView;
							      
	D3D12_PRIMITIVE_TOPOLOGY        m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
							      
	UINT					        m_nSlot = 0;
	UINT					        m_nVertices = 0;
	UINT					        m_nStride = 0;
	UINT					        m_nOffset = 0;
							      
	UINT					        m_nIndices = 0;
	UINT					        m_nStartIndex = 0;
	UINT                            m_nType = 0;
	int						        m_nBaseVertex = 0;
} MeshInfo;

typedef struct MESHLOADINFO
{
	char                            m_pstrMeshName[256] = { 0 };
			             	      
	UINT	             	        m_nType = 0x00;
			             	      
	XMFLOAT3             	        m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3             	        m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);
			             	      
	int		             	        m_nVertices = 0;
							      
	XMFLOAT3                      * m_pxmf3Positions = NULL;
	XMFLOAT4                      * m_pxmf4Colors = NULL;
	XMFLOAT3                      * m_pxmf3Normals = NULL;
	XMFLOAT2                      * m_pxmf2TexCoords = NULL;
			             	      
	int		             	        m_nIndices = 0;
	UINT                          * m_pnIndices = NULL;
			             	      
	int                             m_nSubMeshes = 0;
	int                           * m_pnSubSetIndices = NULL;
	UINT                         ** m_ppnSubSetIndices = NULL;
} MeshLoadInfo;


class CStandardVertex
{
public:
	XMFLOAT3				        m_xmf3Position;
	XMFLOAT3				        m_xmf3Normal;
	XMFLOAT2				        m_xmf2TexCoord;

public:
	CStandardVertex()
	{
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f);
	}
	~CStandardVertex() { }
};

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
class CStandardMesh : public CMesh
{
public:
	CStandardMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, MeshLoadInfo* pMeshInfo);
	virtual ~CStandardMesh();

	virtual void ReleaseUploadBuffers();

protected:

	int						       m_nSubMeshes = 0;
	int                            * m_pnSubSetIndices = NULL;

	ID3D12Resource                 ** m_ppd3dSubSetIndexBuffers = NULL;
	ID3D12Resource                 ** m_ppd3dSubSetIndexUploadBuffers = NULL;
	D3D12_INDEX_BUFFER_VIEW        * m_pd3dSubSetIndexBufferViews = NULL;

public:
	void LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);
};

#define SKINNED_ANIMATION_BONES		128

class CSkinnedMesh : public CStandardMesh
{
public:
	CSkinnedMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CSkinnedMesh();

protected:
	int								m_nBonesPerVertex = 4;

	XMINT4* m_pxmn4BoneIndices = NULL;
	XMFLOAT4* m_pxmf4BoneWeights = NULL;

	ID3D12Resource* m_pd3dBoneIndexBuffer = NULL;
	ID3D12Resource* m_pd3dBoneIndexUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBoneIndexBufferView;

	ID3D12Resource* m_pd3dBoneWeightBuffer = NULL;
	ID3D12Resource* m_pd3dBoneWeightUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBoneWeightBufferView;

public:
	int								m_nSkinningBones = 0;

	char(*m_ppstrSkinningBoneNames)[64];
	CGameObject** m_ppSkinningBoneFrameCaches = NULL; //[m_nSkinningBones]

	XMFLOAT4X4* m_pxmf4x4BindPoseBoneOffsets = NULL; //Transposed

	ID3D12Resource* m_pd3dcbBindPoseBoneOffsets = NULL;
	XMFLOAT4X4* m_pcbxmf4x4MappedBindPoseBoneOffsets = NULL;

	ID3D12Resource* m_pd3dcbSkinningBoneTransforms = NULL;
	XMFLOAT4X4* m_pcbxmf4x4MappedSkinningBoneTransforms = NULL;

public:
	void PrepareSkinning(CGameObject* pModelRootObject);

	void LoadSkinInfoFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
};