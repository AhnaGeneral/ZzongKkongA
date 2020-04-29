#include "Monster.h"
#include "Mgr_Collision.h"

void CMonster::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	Animate(fTimeElapsed, pxmf4x4Parent);

	UpdateCollisionBoxes(pxmf4x4Parent, &m_xmf4Rotation, &m_xmf3Scale);
	CCollisionMgr::GetInstance()->MonsterAttackCheck(m_iAtt, *m_pAttCollision);
}
