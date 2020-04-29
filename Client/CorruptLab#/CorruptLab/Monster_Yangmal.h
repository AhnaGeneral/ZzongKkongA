#pragma once
#include "Monster.h"

class CYangmal : public CMonster
{
public:
	CYangmal() {}
	virtual ~CYangmal() {}

	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
private:
	CGameObject* m_pNeck = NULL;
};

