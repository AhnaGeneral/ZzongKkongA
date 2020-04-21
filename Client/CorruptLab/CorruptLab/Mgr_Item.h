#pragma once
#include "stdafx.h"
using namespace std;

class CPlayer;
class CGameScene;
class CRotatingItem;

class CItemMgr
{

private:
	static CItemMgr* m_pInstance;
	CPlayer* m_pPlayer = NULL;
	CGameScene* m_pScene = NULL;
	CRotatingItem* m_pRotatingItem = NULL;

public:
	void Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	static CItemMgr* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CItemMgr();
		return m_pInstance;
	}
	void Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	void SetPlayer(CPlayer* player) { m_pPlayer = player; }
	void SetScene(CGameScene* scene) { m_pScene = scene; }

};

