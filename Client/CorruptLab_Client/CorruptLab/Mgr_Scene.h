#pragma once
#include "stdafx.h"

class CGameFramework;

class CSceneMgr
{
private:
	UINT* m_iSceneState = NULL;
	static CSceneMgr* m_pInstance;
	CGameFramework* m_pGameFramework = NULL;
public:

	void ChanegeSceneState(int _state);
	void SetSceneStatePointer(UINT* _pScenestate) { m_iSceneState = _pScenestate; }
	void Initialize(CGameFramework* pFramework);
	void Update(float fElapsedTime);

	static CSceneMgr* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CSceneMgr();
		return m_pInstance;
	}
	void Destroy();
};
