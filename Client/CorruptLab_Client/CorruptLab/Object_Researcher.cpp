#include "Object_Researcher.h"

void CResearcher::Update(float fElapsedTime)
{
	if(m_iState == RESEARCHER_IDLE)
		SetAnimationSet(0, m_iTrackNumber);

	Animate(fElapsedTime);
}
