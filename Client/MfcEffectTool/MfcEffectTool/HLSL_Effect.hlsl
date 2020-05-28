#include "Shaders.hlsl"

struct VS_EFFECT_INPUT
{
	float3 position : POSITION;
};

struct GS_EFFECT_OUTPUT
{
	float4 posH :SV_POSITION;
	float3 posW :POSITION;
	float2 UV : TEXCOORD0;
};

struct GS_EFFECT_INPUT
{
	float3 positionW : POSITION;
};

cbuffer cbEffectElementBuffer : register(b20)
{
	float3 tranlationPos : packoffset (c0); 
}

GS_EFFECT_INPUT EffectVertexShader(VS_EFFECT_INPUT input)
{
	GS_EFFECT_INPUT output;
	input.position.x = tranlationPos.x; 
	input.position.y = tranlationPos.y;
	input.position.z = tranlationPos.z;

	output.positionW = input.position;

	return output;
}


[maxvertexcount(4)]
void EffectGS(point GS_EFFECT_INPUT input[1], inout TriangleStream<GS_EFFECT_OUTPUT> outStream)
{
	float3 vUP = float3(0.0f, 1.0f, 0.0f);
	float3 vLook = gvCameraPosition.xyz - input[0].positionW;
	vLook = normalize(vLook);
	float3 vRight = cross(vUP, vLook);

	float fHalfW = 50 * 0.5f;
	float fHalfH = 50 * 0.5f;

	float4 pVertices[4];
	pVertices[0] = float4(input[0].positionW + fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[1] = float4(input[0].positionW + fHalfW * vRight + fHalfH * vUP, 1.0f);
	pVertices[2] = float4(input[0].positionW - fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[3] = float4(input[0].positionW - fHalfW * vRight + fHalfH * vUP, 1.0f);

	float2 pUVs[4] = { float2 (0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

	GS_EFFECT_OUTPUT output;

	for (int i = 0; i < 4; ++i)
	{
		output.posW = pVertices[i].xyz;
		output.posH = mul(mul(pVertices[i], gmtxView), gmtxProjection); 
		output.UV = pUVs[i]; 
		outStream.Append(output);
	}
}

PS_NONLIGHT_MRT_OUTPUT EffectPixelShader(GS_EFFECT_OUTPUT input)
{

	PS_NONLIGHT_MRT_OUTPUT output;

	float3 uvw = float3 (input.UV, 0.0f);
	float4 cColor = gtxtEffectTexture.Sample(gSamplerState, uvw);
	output.NonLight = cColor; 
	return(output);
}
