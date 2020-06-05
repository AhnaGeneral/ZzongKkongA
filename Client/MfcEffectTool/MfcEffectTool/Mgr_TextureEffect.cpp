#include "Mgr_TextureEffect.h"
#include "Object_Effect.h" 
#include "Shader_Effect.h"

CMgr_EffectTex* CMgr_EffectTex::m_pInstance = NULL;

void CMgr_EffectTex::Initialize(ID3D12Device* pd3dDevice, 
	ID3D12GraphicsCommandList* pd3dCommandList, 
	ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_pShaderEffect = new CShader_Effect();
	m_pShaderEffect->BuildObjects(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

void CMgr_EffectTex::Update(float fElapsedTime)
{
}

void CMgr_EffectTex::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	m_pShaderEffect->Render(pd3dCommandList, pCamera);
}

XMFLOAT3* CMgr_EffectTex::SetPosition()
{
	return &(m_pShaderEffect->m_pTestEffectObject->m_TranslationPos);
}

float* CMgr_EffectTex::SetSizeX()
{
	return &(m_pShaderEffect->m_pTestEffectObject->m_sizeX);
}

float* CMgr_EffectTex::SetSizeZ()
{
	return &(m_pShaderEffect->m_pTestEffectObject->m_sizeZ);
}

void CMgr_EffectTex::Destroy()
{
}
