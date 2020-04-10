#include "Monster_Yangmal.h"

void CYangmal::OnInitialize()
{

	m_pNeck = m_pChild->FindFrame("Bip001_Neck");
	m_pAttCollision = &(m_pChild->m_pBoundingBoxes[1]);
	m_pAttCollision->m_pParent = m_pNeck;

}

void CYangmal::Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
	SetAnimationSet(0);
	CGameObject::Animate(fTimeElapsed, NULL);
}
