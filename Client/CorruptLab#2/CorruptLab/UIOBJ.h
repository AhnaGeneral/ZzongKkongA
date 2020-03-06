#pragma once 
#include "Object.h"

class CUIOBJ : public CGameObject
{
public:
	CUIOBJ(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CUIOBJ();
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
};