
#include "HLSL_Noise.hlsl"

// FogAlpha.hlsl =========================================================

struct VS_FOG_INPUT
{
	float3 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PS_FOG_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
};

PS_FOG_INPUT FogVertexShader(VS_FOG_INPUT input)
{
	PS_FOG_INPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f),
		gmtxGameObject), gmtxView), gmtxProjection);

	output.tex = input.tex;

	output.tex1 = (input.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (input.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	return output;
}

PS_NONLIGHT_MRT_OUTPUT FogPixelShader(PS_FOG_INPUT input)
{
	PS_NONLIGHT_MRT_OUTPUT output; 

	float4 noise1 = gtxtAlpha01.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtAlpha02.Sample(gSamplerState, input.tex2);

	float4 finalNoise = lerp (noise1, noise2, 0.7);
	float4 alphaColor = gtxtFinalAlpha.Sample(gSamplerClamp, input.tex);
	output.NonLight = float4(1, 1, 1, alphaColor.a * (finalNoise.a * 1.5));

	return output;

}