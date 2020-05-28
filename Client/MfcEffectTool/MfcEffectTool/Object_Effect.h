#pragma once
#include "Object.h"

struct CB_EFFECT_ELEMENT
{
	XMFLOAT3 TranslationPos; 
};
class CObject_Effect : public CGameObject
{
public:
	CObject_Effect();
	CObject_Effect(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature, XMFLOAT3 position, CShader* pShader = NULL);
	virtual ~CObject_Effect();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
	void TranslationUpdate(XMFLOAT3 pos); 

protected:
	ID3D12Resource            * m_pd3dPositionBuffer = NULL;
	ID3D12Resource            * m_pd3dPositionUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW    m_d3dPositionBufferView;

	ID3D12Resource            * m_pd3dEffectElementBuffer = NULL;
	CB_EFFECT_ELEMENT         * m_pcbEffectElementBuffer = NULL; 

public :
	XMFLOAT3                    m_TranslationPos = XMFLOAT3(30.0f, 0.0f ,40.0f); 

};