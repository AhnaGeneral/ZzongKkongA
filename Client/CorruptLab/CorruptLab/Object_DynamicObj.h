#pragma once
#include "Object_StaticObj.h"
class CDynamicObject :
	public CStaticObject
{
public:

	virtual void Update(float fElapsedTime) {}
};

