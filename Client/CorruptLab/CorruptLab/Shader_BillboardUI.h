#pragma once
#include "Shader_Minimap.h"
class CBillboardUIShader :
	public CMinimapShader
{
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);

};

