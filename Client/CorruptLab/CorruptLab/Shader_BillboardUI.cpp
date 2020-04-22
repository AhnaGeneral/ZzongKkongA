#include "Shader_BillboardUI.h"

D3D12_SHADER_BYTECODE CBillboardUIShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"HLSL_UI.hlsl", "BillboardUI_VS", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CBillboardUIShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"HLSL_UI.hlsl", "BillboardUI_PS", "ps_5_1", ppd3dShaderBlob));
}