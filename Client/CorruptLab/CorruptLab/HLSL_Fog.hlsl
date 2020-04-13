
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
	float4 vPorjPos : TEXCOORD3;
};

Texture2D gtxtSceneDepthTexture : register(t22);

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
	output.vPorjPos = output.position;
	return output;
}

PS_NONLIGHT_MRT_OUTPUT FogPixelShader(PS_FOG_INPUT input)
{
	PS_NONLIGHT_MRT_OUTPUT output; 

	float4 noise1 = gtxtAlpha01.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtAlpha02.Sample(gSamplerState, input.tex2);
	float2 f2pos = float2(input.position.x /380/2 -1, input.position.y/300/ 2-1);
	float myDepth = input.vPorjPos.z / input.vPorjPos.w; 
	float4 fSceneDepth = gtxtSceneDepthTexture.Sample(gSamplerState,f2pos);

		
	
	float4 finalNoise = lerp (noise1, noise2, 0.7);
	float4 alphaColor = gtxtFinalAlpha.Sample(gSamplerClamp, input.tex);
	output.NonLight = float4(1, 1, 1, alphaColor.a * (finalNoise.a * 1.5));
	float fDepthDistance = fSceneDepth.r - myDepth; ;
	if ((fDepthDistance > 0.03f) && (fDepthDistance > 0))
	{                
		fDepthDistance = 1 - fDepthDistance; // 0.7 //0.8 //0.9 
		fDepthDistance /= 1.2;
		//fDepthDistance *= 100; //0.3  //0.2 // 0.1 ..
		//fDepthDistance/= 
		output.NonLight.a -= fDepthDistance;
	}
	//output.NonLight = fSceneDepth;
	return output;

}