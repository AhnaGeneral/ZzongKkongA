#pragma once
#include "Object.h"
class CUI :
	public CGameObject
{
public:
	CUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth, float fxPosition, float fyPosition, float fzPosition);
	~CUI() {}
	
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);


public:
	void Set2DPosition(float x, float y);
};

