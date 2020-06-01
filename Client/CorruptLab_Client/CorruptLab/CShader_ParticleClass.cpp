//#include "CShader_ParticleClass.h"
//
//Shader_ParticleClass::Shader_ParticleClass()
//{
//}
//
//Shader_ParticleClass::Shader_ParticleClass(const Shader_ParticleClass&)
//{
//}
//
//Shader_ParticleClass::~Shader_ParticleClass()
//{
//}
//
//D3D12_INPUT_LAYOUT_DESC Shader_ParticleClass::CreateInputLayout()
//{
//	UINT nInputElementDescs = 3;
//	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
//
//	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//	pd3dInputElementDescs[2] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
//
//	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
//	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
//	d3dInputLayoutDesc.NumElements = nInputElementDescs;
//
//	return d3dInputLayoutDesc;
//}
//
//D3D12_DEPTH_STENCIL_DESC Shader_ParticleClass::CreateDepthStencilState()
//{
//	return D3D12_DEPTH_STENCIL_DESC();
//}
//
//D3D12_SHADER_BYTECODE Shader_ParticleClass::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
//{
//	return D3D12_SHADER_BYTECODE();
//}
//
//D3D12_SHADER_BYTECODE Shader_ParticleClass::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
//{
//	return D3D12_SHADER_BYTECODE();
//}
//
//D3D12_BLEND_DESC Shader_ParticleClass::CreateBlendState()
//{
//	return D3D12_BLEND_DESC();
//}
//
//void Shader_ParticleClass::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets)
//{
//}
