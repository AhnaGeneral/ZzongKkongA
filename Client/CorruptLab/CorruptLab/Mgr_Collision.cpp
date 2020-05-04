
#include "Mgr_Collision.h"
#include "Object_Player.h"
#include "Monster.h"

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
	m_fTime += fTimeElapsed;

	if (box.boundingBox.Intersects(playerBodybox->boundingBox))
	{
		if (m_fTime > 0.5f)
		{
			m_pPlayer->GetDamaage(iDamaege);
			m_fTime = 0.0f;
		}
	}
}

void CCollisionMgr::MonsterDamageCheck(int iDamage)
{
	if (m_pMonsterLists) // ∏ÛΩ∫≈Õ Render
	{
		for (int i = 0; i < 1; i++)
		{
			for (auto Obj : *m_pMonsterLists[i])
			{
				if (!Obj->m_bRender || Obj->m_iState == MONSTER_STATE_STUN || Obj->m_iState == MONSTER_STATE_DAMAGEING) continue;
				
				if (Obj->GetDistanceToPlayer() < 10)
				{
					Obj->GetDamaage(iDamage);
				}
			}
		}
	}

}

