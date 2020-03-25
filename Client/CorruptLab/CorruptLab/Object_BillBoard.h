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

//Nosie ===========================================================================================================
struct CB_NOISEBUFFERTYPE
{
	float       frameTime;
	XMFLOAT3    scrollSpeeds;
	XMFLOAT3    scales;
	float       padding;
};

struct CB_DISTORTIONBUFFERTYPE
{
	XMFLOAT2    distortion1;
	XMFLOAT2    distortion2;
	XMFLOAT2    distortion3;
	float       distortionScale;
	float       distortionBias;
};

class CObjectNosie : public CGameObject
{
protected:
	CB_DISTORTIONBUFFERTYPE m_cbvdistortion;
	CB_NOISEBUFFERTYPE      m_cbvNoisebuffer;

public:
	CObjectNosie(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CObjectNosie();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);

	void UpdateShaderNoiseBuffer(float Time, XMFLOAT3 scrollspeed, XMFLOAT3 scales, float padding);

	virtual void GenerateShaderDistortionBuffer();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);

	virtual void NoiseSetTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
public:
	ID3D12Resource* m_pd3dcbNoiseBuffer = NULL;
	CB_NOISEBUFFERTYPE* m_pcbMappedNoiseBuffers = NULL;

	ID3D12Resource* m_pd3dcbDistortionBuffer = NULL;
	CB_DISTORTIONBUFFERTYPE* m_pcbMappdeDistortBuffers = NULL;
};

class CObjectFog : public CObjectNosie
{
public:
	CObjectFog(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CObjectFog();

	virtual void NoiseSetTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
};