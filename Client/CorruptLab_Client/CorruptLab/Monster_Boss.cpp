#include "Monster_Boss.h"

void CBoss::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent, void* pContext)
{
	SetAnimationSet(0, m_iTrackNumber);
}
``