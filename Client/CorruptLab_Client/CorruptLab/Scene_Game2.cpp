#include"Scene_Game2.h"
#include "stdafx.h"
#include "Mgr_Collision.h"
#include "Object_Floor.h"



//Scene2-----------------------------------------------------------------------------
CGameScene2::CGameScene2()
{
	m_pPlayer = NULL;
	m_pTerrain = NULL;
	m_pSkyBox = NULL;
	m_pFloor = NULL;


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
	m_pFloor = new CFloor(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_pFloor->SetPosition(XMFLOAT3(400, 0, 400));
	m_pShadowCamera = new CSunCamera(XMFLOAT3(400.f, 80.f, -30.f),Vector3::Normalize( XMFLOAT3(0,-1,1.f)));
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
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
	m_pShadowCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CGameScene2::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CGameScene2::ReleaseShaderVariables()
{
}

bool CGameScene2::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		WindowCursorPos = m_ptOldCursorPos;

		//ScreenToClient함수 전체화면 기준인 커서의를 해당 클라이언트 윈도우 기준으로 좌표를 변환해주는 함수 이다. 
		ScreenToClient(hWnd, &WindowCursorPos);
		//n_ReactItem
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}

	return true;
}

bool CGameScene2::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	float* m_fShadowPosition = m_pShadowCamera->GetShadowCameraPosition();

	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0] += 10.0f, m_fShadowPosition[1], m_fShadowPosition[2]);
			break;
		case 'S':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0] -= 10.0f, m_fShadowPosition[1], m_fShadowPosition[2]);
			break;
		case 'D':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0], m_fShadowPosition[1] += 10.0f, m_fShadowPosition[2]);
			break;
		case 'F':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0], m_fShadowPosition[1] -= 10.0f, m_fShadowPosition[2]);
			break;
		case 'G':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0], m_fShadowPosition[1], m_fShadowPosition[2] += 10.0f);
			break;
		case 'H':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0], m_fShadowPosition[1], m_fShadowPosition[2] -= 10.0f);
			break;
		case VK_SPACE:
			m_pPlayer->SetAttackState();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return true;
}

void CGameScene2::DepthRender(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	//if (m_pPlayer) m_pShadowCamera->Update(m_pPlayer->GetCamera());
	m_pShadowCamera->SetViewportsAndScissorRects(pd3dCommandList);
	m_pShadowCamera->Update(m_pPlayer->GetCamera());
	if (m_pShadowCamera) m_pShadowCamera->UpdateShaderVariables(pd3dCommandList,ROOT_PARAMETER_SHADOWCAMERA);

	if (m_pFloor) m_pFloor->Render(pd3dCommandList, pCamera, 1);
	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera, 1);
}

void CGameScene2::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);
	if (m_pFloor) m_pFloor->Render(pd3dCommandList, pCamera);
	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera);
}

void CGameScene2::Update(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
	if (m_pPlayer)
	{
		m_pPlayer->Update(fTimeElapsed);
		m_pPlayer->Animate(fTimeElapsed, NULL);
	}
}

bool CGameScene2::ProcessInput(UCHAR* pKeysBuffer, HWND hWnd)
{
	DWORD dwDirection = 0;
	if (pKeysBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
	if (pKeysBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
	if (pKeysBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
	if (pKeysBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
	if (pKeysBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
	if (pKeysBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;

	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;

	if (GetCapture() == hWnd)
	{
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}

	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			if (pKeysBuffer[VK_RBUTTON] & 0xF0)
			{
				m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
				m_pShadowCamera->Rotate(cyDelta, 0.0f, -cxDelta);

			}
			else
			{
				m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
			}
		}
		if (dwDirection)
			m_pPlayer->Move(dwDirection, m_fElapsedTime, true);
	}
	m_pPlayer->Update(m_fElapsedTime);

	return true;
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
