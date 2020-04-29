#pragma once
#include "Object_Terrain.h"

// SkyBox =========================================================================================================
class CSkyBox : public CGameObject
{
public:
	CSkyBox(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CSkyBox();

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
};

//Cloud =========================================================================================================
class CBillboard : public CGameObject
{
public:
	CBillboard(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		float fWidth, float fHeight, float fDepth, float fPosX, float fPosY, float fPosZ);
	virtual ~CBillboard();

	virtual void Animates(float fTimeElapsed, CCamera* pCamera); // Gameobject와 다른 애니메이트
	void SetLookAt(XMFLOAT3& xmf3TargetCamera);
};
