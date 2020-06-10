#include "Mgr_Scene.h"
#include "GameFramework.h"
CSceneMgr* CSceneMgr::m_pInstance = NULL;
void CSceneMgr::Initialize(CGameFramework* pFramework)
{
	m_pGameFramework = pFramework;
}

void CSceneMgr::Update(float fElapsedTime)
{
}

void CSceneMgr::Destroy()
{
}

void CSceneMgr::ChanegeSceneState(int _state)
{
	if (_state == SCENE_STAGE_INDOOR)
		m_pGameFramework->TurnToIndoorState();
}
