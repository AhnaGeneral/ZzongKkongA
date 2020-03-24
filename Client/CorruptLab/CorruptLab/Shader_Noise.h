#pragma once
#include "Shader.h"

class Shader_Noise: public CShader
{
public:
    Shader_Noise(); 
    virtual ~Shader_Noise(); 

public:

    virtual D3D12_SHADER_BYTECODE    CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
    virtual D3D12_SHADER_BYTECODE    CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
    virtual D3D12_INPUT_LAYOUT_DESC  CreateInputLayout();

    virtual D3D12_BLEND_DESC         CreateBlendState();

    virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets = 1);
    virtual void ReleaseObjects();

};

