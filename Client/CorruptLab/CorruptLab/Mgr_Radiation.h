#pragma once
#include "stdafx.h"
using namespace std;

class CPlayer;
class CGameScene;

class CRadationMgr
{
private:
	static CRadationMgr* m_pInstance;
	CPlayer* m_pPlayer;
	int		m_iRadiation = 34;
	float	m_fTick = 0;

public:
	int GetRaditaion() { return m_iRadiation; }
	void Initialize(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignatureCPlayer);
	void Update(float fElapsedTime);
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

	static CRadationMgr* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CRadationMgr();
		return m_pInstance;
	}
	void Destroy();

};

