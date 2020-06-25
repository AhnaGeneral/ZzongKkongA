#pragma once
#include "Monster.h"
class CTosm :
    public CMonster
{
public:
    CTosm() {}
    ~CTosm() {}

    virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL, void* pContext = NULL);

};

