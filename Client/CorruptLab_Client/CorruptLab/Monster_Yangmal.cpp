#include "Monster_Yangmal.h"
#include "Animation.h"
void CYangmal::OnInitialize()
{

	m_pNeck = m_pChild->FindFrame("Bip001_Neck");
	if (m_pChild->m_pBoundingBoxes)
	{
		m_pAttCollision = &(m_pChild->m_pBoundingBoxes[1]);
		m_pAttCollision->m_pParent = m_pNeck;
	}

}

void CYangmal::Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent, int iNum)
{
	//SetAnimationSet(1);
	switch (m_iState)
	{
	case MONSTER_STATE_STUN:
		if (m_pChild->m_pAnimationController->m_pAnimationTracks[m_iTrackNumber].m_pAnimationSet->m_fPosition >= m_pChild->m_pAnimationController->m_pAnimationTracks[m_iTrackNumber].m_pAnimationSet->m_fLength)
			return;
	default:
		CGameObject::Animate(fTimeElapsed, NULL,iNum);
		break;
	}
}
