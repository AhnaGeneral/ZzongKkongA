#pragma once
#include "stdafx.h"
using namespace std;


class CObject_Effect;
class CShader;
class CGameObject;
class CCamera;

struct EffectTransform
{
	XMFLOAT3 xmf3Conversion_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3Conversion_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3Conversion_Size = XMFLOAT3(1.0f, 1.0f, 1.0f);
	float    fConversion_Alpha = 1.0f; 

	XMFLOAT3 xmf3Default_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3Default_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3Default_Size = XMFLOAT3(1.0f, 1.0f, 1.0f);
	float    fDefault_Alpha = 1.0f;


	XMFLOAT3 xmf3Current_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3Current_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3Current_Size = XMFLOAT3(1.0f, 1.0f, 1.0f);
	float    fCurrent_Alpha = 1.0f;


	XMFLOAT3 xmf3Speed_Rotation = XMFLOAT3(0.1f, 0.1f, 0.1f);
	XMFLOAT3 xmf3Speed_Position = XMFLOAT3(0.1f, 0.1f, 0.1f);
	XMFLOAT3 xmf3Speed_Size = XMFLOAT3(0.05f, 0.05f, 0.05f);
	float    fSpeed_Alpha = 0.001f;

	float    fLifeTime = 0.0f; 
};


class CMgr_EffectMesh
{
private:
	static CMgr_EffectMesh* m_pInstance;
	CGameObject** m_ppMeshEffect = nullptr; 

	CGameObject* m_pMeshEffect = nullptr;

public:
	EffectTransform  m_EffectTransform; 
	bool             m_BoolControl = false; 
	float            m_CountTime = 0.0f; 

public:
	void Initialize(ID3D12Device* pd3dDevice, 
		ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature);
	
	XMFLOAT3* Get_Conversion_RotationPoint();
	XMFLOAT3* Get_Conversion_PositionPoint();
	XMFLOAT3* Get_Conversion_SizePoint();
	float* Get_Conversion_Alpha();

	XMFLOAT3* Get_Default_RotationPoint();
	XMFLOAT3* Get_Default_PositionPoint();
	XMFLOAT3* Get_Default_SizePoint();
	float* Get_Default_Alpha();

	void Set_LifeTime(float LifeTime); 
	
	float  GetCount() { return m_CountTime; }

	XMFLOAT3* Get_Current_RotationPoint();
	XMFLOAT3* Get_Current_PositionPoint();
	XMFLOAT3* Get_Current_SizePoint();
	float*    Get_Current_Alpha();
	

	XMFLOAT3* Get_Speed_RotationPoint();
	XMFLOAT3* Get_Speed_PositionPoint();
	XMFLOAT3* Get_Speed_SizePoint();
	float*    Get_Speed_Alpha();

	bool*     GetBoolControl(); 
	void      FileSave(string FileName); 

	void SetAlpha(float);
	void EffectFileLoad(); 
	void Update(float fTimeElapsed); 
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	static CMgr_EffectMesh* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CMgr_EffectMesh();
		return m_pInstance;
	}
	void Destroy();
};
