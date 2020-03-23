#pragma once 
#include "Object.h"

class CUI : public CGameObject
{
private : 
	UINT m_nobjectID; 
public :
	CUI(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CUI();
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);

	void SetObjectID(UINT objectID);
	virtual UINT GetObjectID() { return m_nobjectID; }
	void Set2DPosition(float x, float y); 
};