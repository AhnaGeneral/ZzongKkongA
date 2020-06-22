#include "stdafx.h"
#include "Scene_Game2.h"
#include "Mgr_Collision.h"
#include "Object_Floor.h"
#include "Object_DynamicObj.h"
#include "Shader_Standard.h"




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

	m_AnimationTime = 0.0f; 
	m_AnimationControl = true;
}

CGameScene2::~CGameScene2()
{
}

void CGameScene2::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pFloor = new CFloor(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_pFloor->SetPosition(XMFLOAT3(0, 0, 0));
	m_pShadowCamera = new CSunCamera(XMFLOAT3(-20.f, 150.f, 0),Vector3::Normalize( XMFLOAT3(0,-1,0.01f)));

	CShader* TexcoordShader = new CTexcoordStandardShader();
	TexcoordShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature, FINAL_MRT_COUNT);
	TexcoordShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	TexcoordShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 3); //16

	m_IndoorWall = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice,
		pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/IndoorWall.bin", TexcoordShader, 0);
	m_IndoorWall->SetPosition(0.f, 0.0f, 0.f);
	m_IndoorWall->SetScale(63.53762f, 21.115f, 77.93047f);
	//m_IndoorWall->SetShader(0, TexcoordShader);

	PlaceObjectsFromFile(pd3dDevice, m_pd3dGraphicsRootSignature, pd3dCommandList);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CGameScene2::ReleaseObjects()
{
	if (m_pFloor)
	{
		m_pFloor->ReleaseUploadBuffers();
		m_pFloor->Release();
		m_pFloor = nullptr; 
	}
	if (m_IndoorWall)
	{
		m_IndoorWall->ReleaseUploadBuffers();
		m_IndoorWall->Release();
		m_IndoorWall = nullptr;
	}
	if (m_pStaticObjLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		{
			for (auto Obj : *m_pStaticObjLists[i])
			{
				Obj->Release();
			}
			m_pStaticObjLists[i]->clear();
		}
	}
	delete[] m_pStaticObjLists;
	m_pStaticObjLists = NULL;

	//----------------------------------------------
	if (m_pDynamicObjLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		{
			for (auto Obj : *m_pDynamicObjLists[i])
			{
				Obj->Release();
			}
			m_pDynamicObjLists[i]->clear();
		}
	}
	delete[] m_pDynamicObjLists;
	m_pDynamicObjLists = NULL;
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

	if (m_pStaticObjLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		{
			for (auto& obj : *m_pStaticObjLists[i])
			{
				obj->UpdateTransform(NULL);
				obj->Render(pd3dCommandList, pCamera, 1);
			}
		}
	}
	if (m_pDynamicObjLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		{
			for (auto Obj : *m_pDynamicObjLists[i])
			{
				Obj->UpdateTransform(NULL);
				Obj->Render(pd3dCommandList, pCamera, 1);
			}
		}
	}

	if (m_pFloor) m_pFloor->Render(pd3dCommandList, pCamera, 1);
	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera, 1);
}

void CGameScene2::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);
	
	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera);
	
	if (m_pFloor) m_pFloor->Render(pd3dCommandList, pCamera);

	if (m_IndoorWall)
		m_IndoorWall->Render(pd3dCommandList, pCamera); 

	if (m_pStaticObjLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		{
			for (auto& obj : *m_pStaticObjLists[i])
			{
				obj->UpdateTransform(NULL);
				obj->Render(pd3dCommandList, pCamera, 0);
			}
		}
	}
	if (m_pDynamicObjLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		{
			for (auto Obj : *m_pDynamicObjLists[i])
			{
				Obj->Update(m_fElapsedTime);
				Obj->UpdateTransform(NULL);
				Obj->Render(pd3dCommandList, pCamera, 0);
			}
		}
	}
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
	float fTime = fTimeElapsed;

	m_AnimationTime += fTimeElapsed;

	if (m_AnimationTime > 2.5f)
	{
		m_AnimationTime = 0.0f;
		
		if (m_AnimationControl)
		{
			m_AnimationControl = false;
		}
		else
		{
			m_AnimationControl = true;
		}
	}

	if (m_AnimationControl)
	{
		fTime = (-(fTime - 0.002f));
	}
	else
	{
		fTime = (+(fTime - 0.002f));
	}

	if (m_pStaticObjLists)
	{
		for (int i = 1 ; i < 5; i++)
		{
			if (m_pStaticObjLists[i])
			{
				for (auto& obj : *m_pStaticObjLists[i])
				{
					XMFLOAT3 TmpPosition = obj->GetPosition();
					obj->SetPosition(TmpPosition.x, TmpPosition.y + fTime, TmpPosition.z);
				}
			}
		}
	}
}

void CGameScene2::ReleaseUploadBuffers()
{
	if (m_pStaticObjLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		{
			if (m_pStaticObjLists[i])
			{
				for (auto& obj : *m_pStaticObjLists[i])
					obj->ReleaseUploadBuffers();
			}
		}
	}
	//----------------------------------------------
	if (m_pDynamicObjLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		{
			for (auto Obj : *m_pDynamicObjLists[i])
			{
				Obj->ReleaseUploadBuffers();
			}
		}
	}
}
