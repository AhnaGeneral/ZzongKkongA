#pragma once

#include "Scene.h"

class CFloor;
class CGameScene2 : public CGameScene
{
public:
	POINT                       WindowCursorPos;

	CPlayer* m_pPlayer;
	CSkyBox* m_pSkyBox;
	CObjectWater* m_pCObjectWater;
	CFloor		* m_pFloor;

	CShader_Effect* m_pTestEffect;
	int                         n_ReactItem;
	float                       itemRange;

public:
	CGameScene2();
	virtual ~CGameScene2();

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


	void PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList);
	void PlaceStaticObjectsFromFile(CGameObject* pModel, char* FileName, UINT index);
	void PlaceDynamicFromFile(CGameObject* pModel, char* FileName, int index);
	void PlaceMonsterFromFile(CGameObject* pModel, char* FileName, int index, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

private:

	bool					    m_bPipelineStateIndex;
	POINT					    m_ptOldCursorPos;

	int						    m_nDynamicObjectTypeNum;
	int						    m_nStaticObjectTypeNum; // ������Ʈ ���� ����
	int						    m_nMonsterTypeNum;      // ���� ���� ����

	vector<CGameObject*>** m_pStaticObjLists;
	vector<CDynamicObject*>** m_pDynamicObjLists;
	vector<CMonster*>** m_pMonsterLists;
	CSoftParticleShader* m_pSoftParticleShader;
	CShader_SpecialFog* m_pSpecialFogShader;


public:
	CCamera* m_pShadowCamera;
	CTexture* m_pShadowMap;
	CTexture* m_pDepthTex;



};
