#pragma once
#include "stdafx.h"
using namespace std;

class CPlayer;
class CGameScene;

struct RadiationInfo
{
	XMFLOAT2 Position;
	float	Range;
	int		Strength;
	RadiationInfo() {}
	RadiationInfo(XMFLOAT2 pos, float range, int strength)
	{
		Position = pos;
		Range = range;
		Strength = strength;
	}
};

class CRadationMgr
{
private:
	static CRadationMgr* m_pInstance;
	CPlayer* m_pPlayer;
	int		m_iRadiation = 34;
	float	m_fTick = 0;
	RadiationInfo m_Radiations[5];

public:
	int GetRaditaion() { return m_iRadiation; }
	void Initialize();
	void Update(float fElapsedTime);
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	int GetStrength(XMFLOAT3 pos);
	static CRadationMgr* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CRadationMgr();
		return m_pInstance;
	}
	void Destroy();

};

