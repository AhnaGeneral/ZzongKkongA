#pragma once
#include "stdafx.h"
using namespace std;

class CObject_Effect;
class CShader;
class CGameObject;
class CCamera;
struct EffectTransform
{
	XMFLOAT3 xmf3Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3Size = XMFLOAT3(1.0f, 1.0f, 1.0f);
};
class CMgr_EffectMesh
{
private:
	static CMgr_EffectMesh* m_pInstance;
	CGameObject* m_pMeshEffect = nullptr;

public:
	EffectTransform  m_EffectTransform; 

public:
	void Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature);
	
	XMFLOAT3* GetRotationPoint();
	XMFLOAT3* GetPositionPoint();
	XMFLOAT3* GetSizePoint();

	void SetPosition(); 
	void SetSize(); 
	void SetRotation(); 
	//void Update(float fElapsedTime);
	
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	static CMgr_EffectMesh* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CMgr_EffectMesh();
		return m_pInstance;
	}
	void Destroy();
};
