#include "Mgr_Radiation.h"
//#include <random>


CRadationMgr* CRadationMgr::m_pInstance = NULL;

void CRadationMgr::Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignatureCPlayer)
{
}

void CRadationMgr::Update(float fElapsedTime)
{
	m_fTick += fElapsedTime;
	
	if (m_fTick > 1.f)
	{
	    int rands = rand() % 10 + 15;
		m_iRadiation = rands;
		m_fTick = 0.f; 
	}
}
void CRadationMgr::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
