#pragma once
#include "Shader.h"

class CStandardShader : public CShader
{
private:
	ID3DBlob* m_pd3dVertexShaderBlob;
	ID3DBlob* m_pd3dPixelShaderBlob;

public:
	CStandardShader();
	virtual ~CStandardShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateShadowVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateShadowPixelShader(ID3DBlob** ppd3dShaderBlob);

	virtual D3D12_BLEND_DESC CreateBlendState();

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
	virtual void CreateBoundingBoxState(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState = 0);
};

class CSkinnedAnimationShader : public CStandardShader
{
public:
	CSkinnedAnimationShader();
	virtual ~CSkinnedAnimationShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateShadowVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
};

class CTransparentedStandardShader : public CStandardShader
{
public:
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

};