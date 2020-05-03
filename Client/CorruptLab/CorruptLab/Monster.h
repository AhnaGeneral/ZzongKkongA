#pragma once
#include "Object_StaticObj.h"

class CUI_MonsterHP;

class CMonster : public CGameObject
{
public:
	CMonster() {}
	virtual ~CMonster();

protected:
	//GameContents

	int				m_iMaxHP = 100;
	int				m_iCurrentHP = 80;
	int				m_iAtt = 5;
	CCollisionBox*	m_pAttCollision;
	CCollisionBox*  m_pBodyCollision;
	CUI_MonsterHP*  m_HPUI;
	///
public:

	int				m_iState = MONSTER_STATE_IDLE;
	//GameContents

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);

	void SetHPUI(CUI_MonsterHP* pHP);
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
	void GetDamaage(int iDamage);
	void GetFullHP() { m_iCurrentHP = m_iMaxHP; }
	///

};

