#include "Shaders.hlsl"

cbuffer DistortionBuffer : register(b10)
{
	float2 distortion1 : packoffset(c0);
	float2 distortion2 : packoffset(c0.z);
	float2 distortion3 : packoffset(c1);
	float distortionScale : packoffset(c1.z);
	float distortionBias : packoffset(c1.w);
};

cbuffer cbNoiseBuffer : register(b9)
{
	float    frameTime : packoffset(c0);
	float3   scrollSpeeds : packoffset(c0.y);
	float3   scales : packoffset (c1);
	float    padding : packoffset (c1.w);
};

struct VS_NOISE_INPUT
{
	float3 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PS_NOISE_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
};


PS_NOISE_INPUT NoiseVertexShader(VS_NOISE_INPUT input)
{
	PS_NOISE_INPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f),
		              gmtxGameObject), gmtxView), gmtxProjection);

	output.tex = input.tex;

	output.tex1 = (input.tex * scales.x);
	output.tex1.y = output.tex1.y + (frameTime * scrollSpeeds.x);

	output.tex2 = (input.tex * scales.y);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	output.tex3 = (input.tex * scales.z);
	output.tex3.y = output.tex3.y + (frameTime * scrollSpeeds.z);

	return output;
}

PS_NONLIGHT_MRT_OUTPUT NoisePixelShader(PS_NOISE_INPUT input) 
{

	PS_NONLIGHT_MRT_OUTPUT output; 

	float4 noise1 = gtxtNoiseTex.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtNoiseTex.Sample(gSamplerState, input.tex2);
	float4 noise3 = gtxtNoiseTex.Sample(gSamplerState, input.tex3);

	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	float4 finalNoise = noise1 + noise2 + noise3;

	float perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	float2 noiseCoords;
	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;

	float4 fireColor = gtxtBaseColorNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);
	float4 alphaColor = gtxtAlphaNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);
	fireColor.a = alphaColor;
	output.NonLight = fireColor;
	return output;
}
