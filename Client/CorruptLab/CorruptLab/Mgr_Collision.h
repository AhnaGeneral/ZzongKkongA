#pragma once

#include "stdafx.h"
using namespace std;

class CCollisionBox;
class CPlayer;
class CMonster;

class CCollisionMgr
{
public:
	CCollisionMgr() {}
	~CCollisionMgr() {}

private:
	static CCollisionMgr* m_pInstance ;
	CPlayer* m_pPlayer = NULL;
	float m_fTime = 0.5f;

public:
	void Initialize(void);
	static CCollisionMgr* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CCollisionMgr();
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

	void InsertCollisionBoxes(BoundingOrientedBox box);
	void SetPlayer(CPlayer* player) { m_pPlayer = player;  }

	bool StaticCollisionCheck();
	void MonsterAttackCheck(int iDamaege, CCollisionBox box, float fTimeElapsed);
	void MonsterDamageCheck(int iDamage);
		
public:
	list<BoundingOrientedBox> m_pStaticCollisionlist;
	list<CMonster*>** m_pMonsterLists;
};

