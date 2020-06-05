#pragma once
#include "stdafx.h"
using namespace std; 

class CObject_Effect; 
class CShader;
class CShader_Effect; 
class CCamera;

class CMgr_EffectTex
{
private: 
	static CMgr_EffectTex* m_pInstance; 
	CShader_Effect* m_pShaderEffect = nullptr;

public:
	void Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature);

	void Update(float fElapsedTime);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera); 
	XMFLOAT3* SetPosition(); 
	float* SetSizeX();
	float* SetSizeZ();

	static CMgr_EffectTex* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CMgr_EffectTex();
		return m_pInstance;
	}

	void Destroy();
};

