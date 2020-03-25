#pragma once
#include "Material.h"
#include "Mesh.h"
#include "Mesh_Skinned.h"
#include "Object_Camera.h"
//#include "Shader_CollisionBox.h"

class CShader;
class CAnimationController;
class Shader_CollisionBox; 

struct CB_GAMEOBJECT_INFO  
{ 
	XMFLOAT4X4 m_xmf4x4World; 
	UINT       m_xnObjectID ; 
};

struct CollisionBox
{
	BoundingOrientedBox boundingBox;
	XMFLOAT3 m_Center;
	XMFLOAT3 m_Extents;
	XMFLOAT4 m_Orientation;
};

class CGameObject
{
private:
	int								m_nReferences = 0;

	XMFLOAT3             	        *m_xmf3AABBCenter;
	XMFLOAT3             	        *m_xmf3AABBExtents;

public:
	void AddRef()  ;
	void Release() ;

public:
	CGameObject();
	virtual ~CGameObject();

public:

	int								m_nBoundingBoxes;
	CollisionBox					*m_pBoundingBoxes = NULL;

	char							m_pstrFrameName[64];

	CMesh                           *m_pMesh = NULL;

	int								m_nMaterials = 0;
	CMaterial                       ** m_ppMaterials = NULL;

	XMFLOAT4X4						m_xmf4x4Transform; // animation ¿¡¼­´Â m_xmf4x4ToParent
	XMFLOAT4X4						m_xmf4x4World;

	CGameObject                     * m_pParent = NULL;
	CGameObject                     * m_pChild = NULL;
	CGameObject                     * m_pSibling = NULL;

	ID3D12Resource                  * m_pd3dcbGameObjects = NULL;	
	CB_GAMEOBJECT_INFO              * m_pcbMappedGameObjects = NULL;

	Shader_CollisionBox             * m_pCollisionBoxShader = NULL; 
	//ID3D12Resource                * m_pd3dCollisionBuffer = NULL;
	//ID3D12Resource                * m_pd3dBoneWeightUploadBuffer = NULL;
	//D3D12_VERTEX_BUFFER_VIEW		m_d3dBoneWeightBufferView;
	
	//Shader_CollisionBox * m

	void SetMesh(CMesh* pMesh);
	void SetShader(CShader* pShader);
	void SetShader(int nMaterial, CShader* pShader);
	void SetMaterial(int nMaterial, CMaterial* pMaterial);

	void SetChild(CGameObject* pChild, bool bReferenceUpdate = false);

	virtual void BuildMaterials(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) { }

	virtual void OnInitialize() { }
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, CMaterial* pMaterial);
	virtual void UpdateCollisionBoxes();

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition() { return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43)); }
	XMFLOAT3 GetLook() { return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33))); }
	XMFLOAT3 GetUp() { return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23)));}
	XMFLOAT3 GetRight() { return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13))); }

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4* pxmf4Quaternion);
	void SetTexture(CTexture* tex);

	CGameObject* GetParent() { return(m_pParent); }
	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);
	CGameObject* FindFrame(char* pstrFrameName);


	UINT GetMeshType() { return((m_pMesh) ? m_pMesh->GetType() : 0); }
	virtual UINT GetObjectID() { return 0; }
	CTexture* FindReplicatedTexture(_TCHAR* pstrTextureName);

public:
	CAnimationController* m_pAnimationController = NULL;

	CGameObject* GetRootSkinnedGameObject();

	void SetAnimationSet(int nAnimationSet);

	void CacheSkinningBoneFrames(CGameObject* pRootFrame);

	void LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CGameObject* pParent, FILE* pInFile, CShader* pShader);
	void LoadBoundingBox(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CGameObject* pGameObject, FILE* pInFile);
	void LoadAnimationFromFile(FILE* pInFile);
	 

	static CGameObject* LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CGameObject* pParent, FILE* pInFile, CShader* pShader);
	static CGameObject* LoadGeometryAndAnimationFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName, CShader* pShader, bool bHasAnimation);
	static void PrintFrameInfo(CGameObject* pGameObject, CGameObject* pParent);

};
