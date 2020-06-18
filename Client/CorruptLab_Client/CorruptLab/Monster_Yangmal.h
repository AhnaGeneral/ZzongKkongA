#pragma once
#include "Monster.h"

class CYangmal :
	public CMonster
{
public:
	CYangmal() {}
	~CYangmal() {}

	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL, int iNum = 0);
private:
	CGameObject* m_pNeck = NULL;
};
