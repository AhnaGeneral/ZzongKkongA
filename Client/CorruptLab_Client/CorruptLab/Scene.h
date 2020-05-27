//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------
#pragma once

#include "Shader.h"
#include "Shader_BillBoard.h"
#include "Object_Player.h"
#include "Object_Nosie.h"
#include "Shader_SpecialFog.h"
using namespace std;


class CHeightMapTerrain;
class CSkyBox;
class CItemBox;
class CShader_Effect; 

class CScene
{
public:

	CScene();
	virtual ~CScene();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void ReleaseObjects() = 0;

	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice) = 0;
	ID3D12RootSignature* GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }
	void SetGraphicsRootSignature(ID3D12GraphicsCommandList* pd3dCommandList) 
	{ pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature); }

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool ProcessInput(UCHAR* pKeysBuffer, HWND hWnd) = 0;

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) = 0;
	virtual void ReleaseShaderVariables() = 0;
	virtual void ReleaseUploadBuffers() = 0;

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL) = 0;
	virtual void Update(float fTimeElapsed) = 0;

protected:
	ID3D12RootSignature       * m_pd3dGraphicsRootSignature;
	float                       m_fElapsedTime;
};


class CMonster;
class CDynamicObject;

class CGameScene : public CScene
{
public:
	POINT                       WindowCursorPos;

	CPlayer                   * m_pPlayer;						      
	CHeightMapTerrain         * m_pTerrain;
	CSkyBox                   * m_pSkyBox;
	CObjectWater              * m_pCObjectWater; 

	CShader_Effect            * m_pTestEffect; 
	int                         n_ReactItem; 
	float                       itemRange;

public:
	CGameScene();
	virtual ~CGameScene();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseObjects();

	void PurifyMonster();


	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void DepthRender(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
	virtual void Update(float fTimeElapsed);

	void ItemBoxCheck();

	virtual bool ProcessInput(UCHAR* pKeysBuffer, HWND hWnd);

	void AnimateObjects(float fTimeElapsed);

	virtual void ReleaseUploadBuffers();

	void CheckCollisions();
	void CheckPlayerCollision();

	void PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList);
	void PlaceStaticObjectsFromFile(CGameObject* pModel, char* FileName, UINT index);
	void PlaceDynamicFromFile(CGameObject* pModel, char* FileName, int index);
	void PlaceMonsterFromFile(CGameObject* pModel, char* FileName, int index, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	void ChangeTerrainPipeline();

private: 

	bool					    m_bPipelineStateIndex;
	POINT					    m_ptOldCursorPos;
							    
	int						    m_nDynamicObjectTypeNum;
	int						    m_nStaticObjectTypeNum; // 오브젝트 종류 개수
	int						    m_nMonsterTypeNum;      // 몬스터 종류 개수

	vector<CGameObject*> 	   ** m_pStaticObjLists; 
	vector<CDynamicObject*>    ** m_pDynamicObjLists;
	vector<CMonster*>          ** m_pMonsterLists;
	CSoftParticleShader         * m_pSoftParticleShader;
	CShader_SpecialFog          * m_pSpecialFogShader;
	

public:
	CCamera                   * m_pShadowCamera;
	CTexture                  * m_pShadowMap;
	CTexture                  * m_pDepthTex;
						      
	

};
