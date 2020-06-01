#include"Scene_Game2.h"
#include "stdafx.h"
#include "Mgr_Collision.h"



//Scene2-----------------------------------------------------------------------------
CGameScene2::CGameScene2()
{
	m_pPlayer = NULL;
	m_pTerrain = NULL;
	m_pSkyBox = NULL;
	m_pCObjectWater = NULL;

	WindowCursorPos = { 0L,0L };

	itemRange = 0.0f;

	m_bPipelineStateIndex = 0;
	m_ptOldCursorPos = { 0L, 0L };

	m_nDynamicObjectTypeNum = 0;
	m_nStaticObjectTypeNum = 0;
	m_nMonsterTypeNum = 0;

	m_pStaticObjLists = NULL;
	m_pDynamicObjLists = NULL;
	m_pMonsterLists = NULL;
	m_pSoftParticleShader = NULL;

	m_pShadowCamera = NULL;

	m_pShadowMap = NULL;
	m_pDepthTex = NULL;
}

CGameScene2::~CGameScene2()
{
}

void CGameScene2::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CGameScene2::ReleaseObjects()
{
}

ID3D12RootSignature* CGameScene2::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	return nullptr;
}

void CGameScene2::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CGameScene2::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CGameScene2::ReleaseShaderVariables()
{
}

bool CGameScene2::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CGameScene2::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CGameScene2::DepthRender(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
}

void CGameScene2::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);
	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera);
}

void CGameScene2::Update(float fTimeElapsed)
{
}

bool CGameScene2::ProcessInput(UCHAR* pKeysBuffer, HWND hWnd)
{
	return false;
}

void CGameScene2::AnimateObjects(float fTimeElapsed)
{
}

void CGameScene2::ReleaseUploadBuffers()
{
}

void CGameScene2::PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CGameScene2::PlaceStaticObjectsFromFile(CGameObject* pModel, char* FileName, UINT index)
{
}

void CGameScene2::PlaceDynamicFromFile(CGameObject* pModel, char* FileName, int index)
{
}

void CGameScene2::PlaceMonsterFromFile(CGameObject* pModel, char* FileName, int index, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}
