#pragma once
#include "Shader.h"
#include "Object_BillBoard.h"

struct GS_BILLBOARD_INSTANCE
{
	XMFLOAT3 m_xmf3Position;
	XMFLOAT2 m_xmf2Size;
};

class CSkyBoxShader : public CShader
{
public:
	CSkyBoxShader();
	virtual ~CSkyBoxShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
};


class CCloudGSShader : public CShader
{
private:

	CMaterial                      * m_pBillboardMaterial;
	int			                     m_nInstances = 0;
	ID3D12Resource                 * m_pd3dInstancesBuffer = NULL;
	ID3D12Resource                 * m_pd3dInstanceUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		 m_d3dInstancingBufferView;
	CBillboard                     * pBillboard = NULL;

public:

	CCloudGSShader();
	virtual ~CCloudGSShader();

	virtual D3D12_SHADER_BYTECODE    CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE    CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE    CreateGeometryShader(ID3DBlob** ppd3dShaderBlob);

	virtual D3D12_RASTERIZER_DESC    CreateRasterizerState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_INPUT_LAYOUT_DESC  CreateInputLayout();
	virtual D3D12_BLEND_DESC         CreateBlendState();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext = NULL);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	virtual void ReleaseObjects();

};


class CObjectNosie;

class CSoftParticleShader :public CShader
{
public:
	CSoftParticleShader() {}
	~CSoftParticleShader() {}

	CTexture* m_pSceneDepthTextures = NULL;
	CTexture* m_pFireNoiseTextures = NULL;
	CTexture* m_pFogNoiseTextures = NULL;

	CObjectNosie					** m_pFireObjects = NULL;
	int			                     m_nFire = 0;

	CObjectNosie**					m_pFogObjects = NULL;
	int			                     m_nFog = 0;

	virtual D3D12_BLEND_DESC         CreateBlendState();
	virtual D3D12_INPUT_LAYOUT_DESC  CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE    CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE    CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE    CreateGeometryShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	D3D12_SHADER_BYTECODE    CreateFogVertexShader(ID3DBlob** ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE    CreateFogPixelShader(ID3DBlob** ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE    CreateFogGeometryShader(ID3DBlob** ppd3dShaderBlob);

	void CreateNoiseTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual D3D12_RASTERIZER_DESC    CreateRasterizerState();
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext = NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	virtual void ReleaseObjects();
};