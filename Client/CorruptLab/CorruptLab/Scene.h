//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------
#pragma once

#include "Shader.h"
#include "Shader_BillBoard.h"
#include "Object_Player.h"
#include "Object_Nosie.h"
using namespace std;


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

	CCamera* m_pShadowCamera = NULL;

protected:
	float m_fElapsedTime;
};



class CGameScene : public CScene
{
public:
	CPlayer              * m_pPlayer = NULL;

	CHeightMapTerrain    * m_pTerrain = NULL;
	CSkyBox              * m_pSkyBox = NULL;
	CCloudGSShader       * m_pCloudGSShader = NULL;
	CUI                  * m_pUIObj = NULL;
	CObjectNosie         * m_pNoiseObject = NULL;
	CObjectFog           * m_pCObjectFog = NULL;
	CObjectWater         * m_pCObjectWater = NULL; 


public:
	CGameScene();
	~CGameScene();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseObjects();


	virtual ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void DepthRender(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
	virtual void Update(float fTimeElapsed);

	virtual bool ProcessInput(UCHAR* pKeysBuffer, HWND hWnd);

	void AnimateObjects(float fTimeElapsed);

	virtual void ReleaseUploadBuffers();

	void CheckCollisions();
	void CheckPlayerCollision();

	void PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList);
	void PlaceObjectsFromFile(CGameObject* pModel, char* FileName, int index);

	void ChangeTerrainPipeline();

private: // 배치되는 오브젝트들

	bool					m_bPipelineStateIndex = 0;
	POINT					m_ptOldCursorPos;
	int						m_nObjectTypeNum; // 오브젝트 종류 개수
	list<CGameObject*>**	m_pStaticObjLists; // list<CGameObject*>*의 배열
};
