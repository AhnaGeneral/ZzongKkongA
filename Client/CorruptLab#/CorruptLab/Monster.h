#pragma once
#include "Object_StaticObj.h"


class CMonster : public CGameObject
{
public:
	CMonster() {}
	virtual ~CMonster() {}

protected:
	//GameContents

	int				m_iState = MONSTER_STATE_IDLE;
	int				m_iMaxHP = 100;
	int				m_iCurrentHP = 100;
	int				m_iAtt = 20;
	CCollisionBox*	m_pAttCollision;
	CCollisionBox*  m_pBodyCollision;

	///
public:

	//GameContents
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
	void GetDamaage(int iDamage) { m_iCurrentHP -= iDamage; }
	void GetFullHP() { m_iCurrentHP = m_iMaxHP; }
	///

};

