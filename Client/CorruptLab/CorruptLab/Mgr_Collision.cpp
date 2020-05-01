
#include "Mgr_Collision.h"
#include "Object_Player.h"

CCollisionMgr* CCollisionMgr::m_pInstance = NULL;

void CCollisionMgr::InsertCollisionBoxes(BoundingOrientedBox box)
{
	m_pStaticCollisionlist.push_back(box);
}

bool CCollisionMgr::StaticCollisionCheck()
{
	CCollisionBox* playerBodybox = m_pPlayer->m_pBodyCollision;
	if (!playerBodybox) return false;
	for (auto objCol : m_pStaticCollisionlist)
	{
		if (objCol.Intersects(playerBodybox->boundingBox))
			return true;
	}
	return false;
}

void CCollisionMgr::MonsterAttackCheck(int iDamaege, CCollisionBox box, float fTimeElapsed)
{
	CCollisionBox* playerBodybox = m_pPlayer->m_pBodyCollision;


	if (box.boundingBox.Intersects(playerBodybox->boundingBox))
	{
		m_fTime += fTimeElapsed;
		if (m_fTime > 0.5f)
		{
			m_pPlayer->GetDamaage(iDamaege);
			m_fTime = 0.0f;
		}
	}
}

