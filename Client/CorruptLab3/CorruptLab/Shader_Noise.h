#pragma once
#include "Shader.h"


//struct NoiseBufferType
//{
//    float frameTime;
//    XMFLOAT3 scrollSpeeds;
//    XMFLOAT3 scales;
//    float padding;
//};
//
//struct DistortionBufferType
//{
//    XMFLOAT2 distortion1;
//    XMFLOAT2 distortion2;
//    XMFLOAT2 distortion3;
//    float distortionScale;
//    float distortionBias;
//};

class Shader_Noise: public CShader
{
public:
    Shader_Noise(); 
    virtual ~Shader_Noise(); 

public:

    virtual D3D12_SHADER_BYTECODE    CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
    virtual D3D12_SHADER_BYTECODE    CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
    virtual D3D12_INPUT_LAYOUT_DESC  CreateInputLayout();

    //virtual D3D12_RASTERIZER_DESC    CreateRasterizerState();
    //virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
    //virtual D3D12_BLEND_DESC         CreateBlendState();

    virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
    virtual void ReleaseObjects();

};

