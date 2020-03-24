#pragma once
#include "Shader_MRT.h"

struct Water_Info
{
	XMFLOAT3 m_xmf3Position;
	XMFLOAT2 m_xmf2Scale;
};
class CWaterShader :	public CShader
{
private:
	CTexture* m_pWaterNormal = NULL;
	Water_Info m_Info;

	ID3D12Resource* m_pd3dInstancesBuffer = NULL;
	ID3D12Resource* m_pd3dInstanceUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		 m_d3dInstancingBufferView;


public:

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** ppd3dShaderBlob);


	virtual D3D12_INPUT_LAYOUT_DESC  CreateInputLayout();

	virtual D3D12_BLEND_DESC CreateBlendState();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext = NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	virtual void ReleaseObjects();

};

