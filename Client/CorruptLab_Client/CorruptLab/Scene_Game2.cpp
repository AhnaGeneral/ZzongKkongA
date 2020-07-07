#include "stdafx.h"
#include "Scene_Game2.h"
#include "Mgr_Collision.h"
#include "Object_Floor.h"
#include "Object_DynamicObj.h"
#include "Shader_Standard.h"
#include "Monster_Boss.h"
#include "Shader_Noise.h"
#include "Shader_BillBoard.h"
#include "Mgr_IndoorControl.h"

//Scene2-----------------------------------------------------------------------------
CGameScene2::CGameScene2()
{
	m_pPlayer = NULL;
	m_pTerrain = NULL;
	m_pSkyBox = NULL;
	m_pFloor = NULL;
	m_pBoss = NULL;

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
	CMgr_IndoorControl::GetInstance()->Initialize();
	//m_pPassWordTexture = NULL; 
	
	//m_pLabatoryPos = new vector<XMFLOAT3>; 
}

CGameScene2::~CGameScene2()
{
}

void CGameScene2::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pFloor = new CFloor(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_pFloor->SetPosition(XMFLOAT3(0, 0, 0));

	XMFLOAT3 vUp = XMFLOAT3(0, 1, 0);
	XMFLOAT3 vRight = XMFLOAT3(-1, 0, 0);
	XMFLOAT3 m_xmf3Look = XMFLOAT3(0, -1, 0.01f);
	XMFLOAT3 m_xmf3Right = Vector3::CrossProduct(vUp, m_xmf3Look);
	XMFLOAT3 m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right);

	m_pShadowCamera = new CSunCamera(XMFLOAT3(-20.f, 150.f, 0), m_xmf3Look, m_xmf3Right, m_xmf3Up);

	CShader* TexcoordShader = new CTexcoordStandardShader();
	TexcoordShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature, FINAL_MRT_COUNT);
	TexcoordShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	TexcoordShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 3); //16
	
	CGameObject* pBossModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice,
		pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Monster/Boss_Idle.bin", NULL, 1);
	
	m_pBoss = new CBoss();
	m_pBoss->SetChild(pBossModel);
	m_pBoss->SetPosition(XMFLOAT3(0, 0, 0));
	m_pBoss->SetScale(5, 5, 5);

	//CShader* PlaneLineShader = new CStandardShader();
	//PlaneLineShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature, FINAL_MRT_COUNT);
	//PlaneLineShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//PlaneLineShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 2); //16

	m_IndoorWall = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice,
		pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Indoor2.bin", TexcoordShader, 0);
	m_IndoorWall->SetPosition(0.f, 0.0f, 0.f);
	m_IndoorWall->SetScale(63.53762f, 21.115f, 77.93047f);

	CTriangleRect* lenghtMesh = new CTriangleRect(pd3dDevice, pd3dCommandList, FRAME_BUFFER_WIDTH / 7.0f,
		FRAME_BUFFER_HEIGHT / 7.f, 0.0f, 1.0f);

	CTriangleRect* wirthMesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 0,
		FRAME_BUFFER_HEIGHT / 7.f, FRAME_BUFFER_WIDTH / 7.0f, 1.0f);

	m_IndoorWallLine = new CGameObject * [4];
	Shader_Basic* pShader = new Shader_Basic();
	pShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature, FINAL_MRT_COUNT);
	pShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 0); //16

	m_IndoorWallLines = new CGameObject();
	m_IndoorWallLines->SetShader(pShader);
	m_IndoorWallLines->SetMesh(wirthMesh);
	m_IndoorWallLines->SetScale(0.f, 0.007f, 1.0f);
	//m_IndoorWallLines->Rotate(0.0f, 10, 0);
	m_IndoorWallLines->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_IndoorWallLines->SetPosition(102, 4.f, 0);

	m_IndoorWallLine[0] = m_IndoorWallLines;
	XMFLOAT3 Axis = XMFLOAT3(0.0f, 1.f, 0.0); 
	m_IndoorWallLines = new CGameObject();
	m_IndoorWallLines->SetShader(pShader);
	m_IndoorWallLines->SetMesh(wirthMesh);
	m_IndoorWallLines->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_IndoorWallLines->SetScale(0.f, 0.007f, 1.0f);
	//m_IndoorWallLines->Rotate(0.0f, 10, 0);
	m_IndoorWallLines->SetPosition(-102, 4.f, 0);

	m_IndoorWallLine[1] = m_IndoorWallLines;
	m_IndoorWallLines = new CGameObject();
	m_IndoorWallLines->SetShader(pShader);
	m_IndoorWallLines->SetMesh(lenghtMesh);
	m_IndoorWallLines->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_IndoorWallLines->SetScale(1.2f, 0.007f, 0.0f);
	m_IndoorWallLines->SetPosition(0.0f, 4.f, 62.6f);

	m_IndoorWallLine[2] = m_IndoorWallLines;
	m_IndoorWallLines = new CGameObject();
	m_IndoorWallLines->SetShader(pShader);
	m_IndoorWallLines->SetMesh(lenghtMesh);
	m_IndoorWallLines->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_IndoorWallLines->SetScale(1.2f, 0.007f, 0.0f);
	m_IndoorWallLines->SetPosition(0.0f , 4.f, -62.0f);
	m_IndoorWallLine[3] = m_IndoorWallLines;

	//m_pPassWordTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	//m_pPassWordTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Noise/fire01.dds", 0);

	//Shader_Password = new  Shader_TextureBasic();
	//Shader_Password->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature, FINAL_MRT_COUNT);
	//Shader_Password->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//Shader_Password->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 1); //16
	//Shader_Password->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pPassWordTexture, ROOT_PARAMETER_EFFECT, false);

	//m_pPasswordobj = new CGameObject();
	//m_pPasswordobj->SetShader(Shader_Password);
	//m_pPasswordobj->SetMesh(lenghtMesh);
	//m_pPasswordobj->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//m_pPasswordobj->SetScale(1.0f, 1.0, 0.0f);
	//m_pPasswordobj->SetPosition(0.0f, 0.0f, 0.0f);

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

	//if (m_PlaneLine)
	//{
	//	m_PlaneLine->ReleaseUploadBuffers();
	//	m_PlaneLine->Release();
	//	m_PlaneLine = nullptr;
	//}
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

	if (m_pBoss) m_pBoss->Release();
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
	for (int i = 0; i < 4; ++i)
	{
		//m_IndoorWallLine[i]->Rotate(0, 10 + (i) * 500, 0);
		m_IndoorWallLine[i]->UpdateShaderVariable(pd3dCommandList, &(m_IndoorWallLine[i]->m_xmf4x4World));
	}
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
		case 'Q':
		case 'q':
			PassWordCheck();
			break;
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
			if (CMgr_IndoorControl::GetInstance()->GetPasswordControl())
			{
		case'1':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(1);
			break;
		case'2':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(2);
			break;
		case'3':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(3);
			break;
		case'4':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(4);
			break;
		case'5':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(5);
			break;
		case'6':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(6);
			break;
		case'7':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(7);
			break;
		case'8':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(8);
			break;
		case'9':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(9);
			break;
		case'0':
			CMgr_IndoorControl::GetInstance()->GetlistPassword()->push_back(0);
			break;
			}
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

	if (m_pBoss) m_pBoss->Render(pd3dCommandList, pCamera, 1);

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

	UpdateShaderVariables(pd3dCommandList);

	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera);
	
	if (m_pFloor) m_pFloor->Render(pd3dCommandList, pCamera);
	
	//if(Shader_Password)Shader_Password->Render(pd3dCommandList, pCamera);
	//if(m_pPassWordTexture)m_pPassWordTexture->UpdateShaderVariable(pd3dCommandList, 0);

	//if (m_pPasswordobj) 
	//{
	//	m_pPasswordobj->UpdateTransform(NULL);
	//	m_pPasswordobj->Render(pd3dCommandList, 0);
	//}

	//if (m_IndoorWall) m_IndoorWall->Render(pd3dCommandList, pCamera); 
	if (m_pBoss)
	{
		m_pBoss->Update(m_fElapsedTime, NULL, NULL);

		m_pBoss->Animate(m_fElapsedTime, NULL, 0);
		//	if (Obj->m_iTrackNumber == 1) continue;
		m_pBoss->UpdateTransform(NULL);
		m_pBoss->Render(pd3dCommandList, pCamera);
	}
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

	for (int i = 0; i < 4; ++i)
	{
		m_IndoorWallLine[i]->UpdateTransform(NULL);
		m_IndoorWallLine[i]->Render(pd3dCommandList, pCamera, 0);
	}


}

void CGameScene2::Update(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
	if (m_pPlayer)
	{
		//m_pPlayer->Update(fTimeElapsed);
		m_pPlayer->Animate(fTimeElapsed, NULL);
	}
}

int tmp = 0; 

void CGameScene2::PassWordCheck()
{
	for (auto pObj : *m_pStaticObjLists[OBJECT_INDOOR_TYPE_PASSWORD])
	{
		XMFLOAT3 ObjPos = pObj->GetPosition();
		XMFLOAT3 PlayerPos = m_pPlayer->GetPosition();

		float Distance = Vector3::Length(Vector3::Subtract(ObjPos, PlayerPos));
		if (Distance < 15)
		{
			if (tmp == 0)
			{
				tmp = 1; 
			}
			else
			{
				tmp = 0;
			}
			CMgr_IndoorControl::GetInstance()->SetpasswordControl(tmp);
		}
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
		for (int i = 1 ; i < 7; i++)
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
