//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include "Shader.h"
#include "MRTShader.h"
#include "Player.h"
#include <list>
using namespace std;

struct LIGHT
{
	XMFLOAT4	      m_xmf4Ambient;
	XMFLOAT4	      m_xmf4Diffuse;
	XMFLOAT4	      m_xmf4Specular;
	XMFLOAT3	      m_xmf3Position;
	float 		      m_fFalloff;
	XMFLOAT3	      m_xmf3Direction;
	float 		      m_fTheta; //cos(m_fTheta)
	XMFLOAT3	      m_xmf3Attenuation;
	float		      m_fPhi; //cos(m_fPhi)
	bool		      m_bEnable;
	int			      m_nType;
	float		      m_fRange;
	float		      padding;
};				      
				      
struct LIGHTS	      
{				      
	XMFLOAT4	      m_xmf4GlobalAmbient;
	LIGHT		      m_pLights[MAX_LIGHTS];
};				      
				      
struct MATERIAL	      
{				      
	XMFLOAT4	      m_xmf4Ambient;
	XMFLOAT4	      m_xmf4Diffuse;
	XMFLOAT4	      m_xmf4Specular; //(r,g,b,a=power)
	XMFLOAT4	      m_xmf4Emissive;
};

struct MATERIALS
{
	MATERIAL	     m_pReflections[MAX_MATERIALS];
};

class CHeightMapTerrain;
class CSkyBox;


class CScene
{
public:
	CScene() {}
	~CScene() {}

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void ReleaseObjects() = 0;

	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;

	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice) = 0;
	ID3D12RootSignature* GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }
	void SetGraphicsRootSignature(ID3D12GraphicsCommandList* pd3dCommandList) { pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature); }

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void ReleaseShaderVariables() = 0;

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL) = 0;
	virtual void Update(float fTimeElapsed) = 0;

	virtual bool ProcessInput(UCHAR* pKeysBuffer, HWND hWnd) = 0;

	virtual void ReleaseUploadBuffers() = 0;

	CCamera* m_pCamera = NULL;

protected:
	float m_fElapsedTime;
};



class CGameScene : public CScene
{
public:
	CPlayer* m_pPlayer = NULL;

	LIGHTS              * m_pLights = NULL;
	CHeightMapTerrain   * m_pTerrain = NULL;
	CSkyBox             * m_pSkyBox = NULL;
	CCloudGSShader      * m_pCloudGSShader = NULL;
	CUIOBJ              * m_pUIObj = NULL;

	ID3D12Resource* m_pd3dcbLights = NULL;
	LIGHTS* m_pcbMappedLights = NULL;

	MATERIALS* m_pMaterials = NULL;

	ID3D12Resource* m_pd3dcbMaterials = NULL;
	MATERIAL* m_pcbMappedMaterials = NULL;

public:
	CGameScene();
	~CGameScene();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseObjects();

	void BuildLightsAndMaterials();

	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
	virtual void Update(float fTimeElapsed);

	virtual bool ProcessInput(UCHAR* pKeysBuffer, HWND hWnd);

	void AnimateObjects(float fTimeElapsed);

	virtual void ReleaseUploadBuffers();

	void SetTerrainPipelineState();
	void PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);


private: // 배치되는 오브젝트들

	POINT					m_ptOldCursorPos;
	int						m_nObjectTypeNum; // 오브젝트 종류 개수
	list<CGameObject*>** m_pObjectLists; // list<CGameObject*>*의 배열
};
