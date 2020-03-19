#pragma once
#include "Shader.h"

struct GS_COLLISION_BOX_INFO
{
	XMFLOAT3  m_xmf3Center;
	XMFLOAT3  m_xmf3Extent;
	XMFLOAT4  m_xmf4Orientation;
};

class Shader_CollisionBox : public CShader
{
private:
	int								 m_nCollisionBoxs = 0;
	ID3D12Resource                 * m_pd3dCollisionBuffer = NULL;
	ID3D12Resource                 * m_pd3dCollisionUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		 m_d3dCollisionBufferView;


public:
	int m_nInstance; 
	GS_COLLISION_BOX_INFO* m_pBoxInfo = NULL; 

	Shader_CollisionBox();
	virtual ~Shader_CollisionBox();

	virtual D3D12_SHADER_BYTECODE    CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE    CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE    CreateGeometryShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_INPUT_LAYOUT_DESC  CreateInputLayout();

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext = NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);


};

