#pragma once 
#include "Object.h"

class CUIOBJ : public CGameObject
{
private : 
	UINT m_nobjectID; 
public:
	CUIOBJ(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CUIOBJ();
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);

	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	void SetObjectID(UINT objectID); 
	virtual UINT GetObjectID() { return m_nobjectID; }
	void Set2DPosition(float x, float y); 
};