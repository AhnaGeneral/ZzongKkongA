#include "EndingScene.h"
#include "Monster_Boss.h"

CEndingScene::CEndingScene(CPlayer* player, CBoss* boss)
{
	m_pPlayer = player;
	m_pBoss = boss;
}

ID3D12RootSignature* CEndingScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	return nullptr;
}

bool CEndingScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CEndingScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CEndingScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{

	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);



	if (m_pPlayer) {
		m_pPlayer->Animate(m_fElapsedTime, NULL);
		m_pPlayer->Render(pd3dCommandList, pCamera);
	}
	if (m_pBoss)
	{
		m_pBoss->Update(m_fElapsedTime, NULL, NULL);

		m_pBoss->Animate(m_fElapsedTime, NULL, 0);
		//	if (Obj->m_iTrackNumber == 1) continue;
		m_pBoss->UpdateTransform(NULL);
		m_pBoss->Render(pd3dCommandList, pCamera);
	}

}

void CEndingScene::Update(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
}

bool CEndingScene::ProcessInput(UCHAR* pKeysBuffer, HWND hWnd)
{
	return false;
}
