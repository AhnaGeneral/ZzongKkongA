#pragma once
#include "Shader_MRT.h"

struct LIGHT
{
	XMFLOAT4	      m_xmf4Ambient;
	XMFLOAT4	      m_xmf4Diffuse;
	XMFLOAT4	      m_xmf4Specular;
	XMFLOAT3	      m_xmf3Position;
	float 		      m_fFalloff;
	XMFLOAT3	      m_xmf3Direction;
	float 		      m_fTheta; //cos(m_fTheta)
	XMFLOAT3	      m_xmf3Attenuation;
	float		      m_fPhi; //cos(m_fPhi)
	bool		      m_bEnable;
	int			      m_nType;
	float		      m_fRange;
	float		      padding;
};

struct LIGHTS
{
	XMFLOAT4	      m_xmf4GlobalAmbient;
	LIGHT		      m_pLights[MAX_LIGHTS];
};

struct MATERIAL
{
	XMFLOAT4	      m_xmf4Ambient;
	XMFLOAT4	      m_xmf4Diffuse;
	XMFLOAT4	      m_xmf4Specular; //(r,g,b,a=power)
	XMFLOAT4	      m_xmf4Emissive;
};

struct MATERIALS
{
	MATERIAL	     m_pReflections[MAX_MATERIALS];
};

class CLightTarget : public CPostProcessingShader
{
public:
	CLightTarget() {}
	~CLightTarget() {}

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext = NULL);

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	void BuildLightsAndMaterials();

protected:
	CTexture* m_pTextures = NULL;

	MATERIALS* m_pMaterials = NULL;
	LIGHTS* m_pLights = NULL;

	ID3D12Resource* m_pd3dcbLights = NULL;
	LIGHTS* m_pcbMappedLights = NULL;

	ID3D12Resource* m_pd3dcbMaterials = NULL;
	MATERIAL* m_pcbMappedMaterials = NULL;

};

