#include "Monster.h"
#include "Mgr_Collision.h"
#include "Object_UI.h"

CMonster::~CMonster()
{
	m_HPUI->ReleaseShaderVariables();
	m_HPUI->Release();
}


void CMonster::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	CGameObject::Render(pd3dCommandList, pCamera, nPipelineState);
	m_HPUI->UpdateTransform(&m_xmf4x4World);
	m_HPUI->Render(pd3dCommandList, pCamera);
}

void CMonster::SetHPUI(CUI_MonsterHP* pHP)
{
	m_HPUI = pHP;
	m_HPUI->m_pcbMonsterHP = &m_iCurrentHP;

}

void CMonster::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	Animate(fTimeElapsed, pxmf4x4Parent);

	UpdateCollisionBoxes(pxmf4x4Parent, &m_xmf4Rotation, &m_xmf3Scale);
	CCollisionMgr::GetInstance()->MonsterAttackCheck(m_iAtt, *m_pAttCollision, fTimeElapsed);
}

void CMonster::GetDamaage(int iDamage)
{
	m_iCurrentHP -= iDamage;
	if (m_iCurrentHP <= 20) 
	{
		m_iState = MONSTER_STATE_STUN;
		m_iCurrentHP = 20;
	}
}
