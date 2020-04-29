#pragma once 
#include "Object.h"

class CMRTUI : public CGameObject
{
private:
	UINT m_nobjectID;
public:
	CMRTUI() {}
	CMRTUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CMRTUI();
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	virtual void InterLinkShaderTexture(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* Texture = NULL) {}
	virtual void ReleaseShaderVariables();
	//virtual void ReleaseUploadBuffers();
	void SetObjectID(UINT objectID);
	virtual UINT GetObjectID() { return m_nobjectID; }
	void Set2DPosition(float x, float y);
};


class CUI_MiniMap : public CMRTUI
{
public:
	CUI_MiniMap();
	CUI_MiniMap(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);

	virtual ~CUI_MiniMap() {}

	virtual void InterLinkShaderTexture(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* Texture = NULL);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void ReleaseShaderVariables();

	//virtual void ReleaseUploadBuffers();
	void SetPlayerPosition(XMFLOAT3* pos) { m_PlayerPosition = pos; }

private:
	ID3D12Resource* m_pd3dcbPlayerPosition = NULL;
	XMFLOAT3* m_PlayerPosition = NULL;
	XMFLOAT3* m_pcbPlayerPosition = NULL;
};

class CUI_Root : public CMRTUI
{
public:
	CUI_Root();
	CUI_Root(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CUI_Root() {}

	virtual void InterLinkShaderTexture(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pShader = NULL, void* pTexture = NULL);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void ReleaseShaderVariables();

};


class CUI_ITem : public CMRTUI
{
private:
	UINT   m_nobjectID;
	UINT* m_ItemReact = NULL;

public:
	CUI_ITem();
	CUI_ITem(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CUI_ITem() {}

	virtual void InterLinkShaderTexture(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* Texture = NULL);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void ReleaseShaderVariables();

	void SetObjectID(UINT objectID) { m_nobjectID = objectID; }
	virtual UINT GetObjectID() { return m_nobjectID; }
	void SetItemReact(UINT* React) { m_ItemReact = React; }

private:
	ID3D12Resource* m_pd3dcbItemReact = NULL;
	UINT* m_pcbItemReact = NULL;
};


class CUI_HP : public CMRTUI
{
public:
	CUI_HP() {}
	CUI_HP(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature) {}

	virtual ~CUI_HP() {}

	virtual void InterLinkShaderTexture(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* Texture = NULL);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void ReleaseShaderVariables();


	void SetPlayerHP(int* playerHPRemaining) { m_PlayerHP = playerHPRemaining; }
	void SetObjectID(UINT objectID) { m_nobjectID = objectID; }
	virtual UINT GetObjectID() { return m_nobjectID; }
	 
private:
	UINT   m_nobjectID;

	ID3D12Resource* m_pd3dcbPlayerHP = NULL;
	int* m_PlayerHP = NULL;
	int* m_pcbPlayerHP = NULL;

};

class CUI_RaditaionLevel : public CMRTUI
{
public:
	CUI_RaditaionLevel();
	CUI_RaditaionLevel(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CUI_RaditaionLevel() {}

	virtual void InterLinkShaderTexture(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pShader = NULL, void* pTexture = NULL);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void ReleaseShaderVariables();
	void SetRadiationNumber(int num);

private:
	//CMesh*			m_pMeshTen = NULL;
	//CMesh*			m_pMeshOne = NULL;

	ID3D12Resource* m_pd3dcbRadiationLevel = NULL;
	int m_RadiationNumber;
	int* m_pcbRadiationNum;
};