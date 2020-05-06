
#include "HLSL_Noise.hlsl"

// FogAlpha.hlsl =========================================================


struct VS_FOG_INPUT
{
	float3 position : POSITION;
};

struct PS_FOG_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float4 vPorjPos : TEXCOORD3;
};

Texture2D gtxtSceneDepthTexture : register(t23);

GS_NOISE_INPUT FogVertexShader(VS_FOG_INPUT input)
{
	GS_NOISE_INPUT output;

	output.positionW = input.position;

	return output;
}


[maxvertexcount(4)]
void GS(point GS_NOISE_INPUT input[1], inout TriangleStream<PS_FOG_INPUT> outStream)
{
	float3 vUP = float3(0.0f, 1.0f, 0.0f);
	float3 vLook = gvCameraPosition.xyz - input[0].positionW;
	vLook = normalize(vLook);
	float3 vRight = cross(vUP, vLook);

	float fHalfW = 100 * 0.8f;
	float fHalfH = 70 * 0.8f;

	float4 pVertices[4];
	pVertices[0] = float4(input[0].positionW + fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[1] = float4(input[0].positionW + fHalfW * vRight + fHalfH * vUP, 1.0f);
	pVertices[2] = float4(input[0].positionW - fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[3] = float4(input[0].positionW - fHalfW * vRight + fHalfH * vUP, 1.0f);

	float2 pUVs[4] = { float2 (0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

	PS_FOG_INPUT output;

	output.position = mul(mul(pVertices[0], gmtxView), gmtxProjection);
	output.vPorjPos = output.position;
	output.tex = pUVs[0];

	output.tex1 = (output.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (output.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	outStream.Append(output);

	output.position = mul(mul(pVertices[1], gmtxView), gmtxProjection);
	output.vPorjPos = output.position;
	output.tex = pUVs[1];

	output.tex1 = (output.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (output.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	outStream.Append(output);

	output.position = mul(mul(pVertices[2], gmtxView), gmtxProjection);
	output.tex = pUVs[2];

	output.tex1 = (output.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (output.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);
	output.vPorjPos = output.position;

	outStream.Append(output);

	output.position = mul(mul(pVertices[3], gmtxView), gmtxProjection);
	output.vPorjPos = output.position;
	output.tex = pUVs[3];

	output.tex1 = (output.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (output.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	outStream.Append(output);


}

PS_NONLIGHT_MRT_OUTPUT FogPixelShader(PS_FOG_INPUT input)
{
	PS_NONLIGHT_MRT_OUTPUT output;

	float4 noise1 = gtxtAlpha01.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtAlpha02.Sample(gSamplerState, input.tex2);
	float2 f2pos = float2(input.position.x / 380 / 2 - 1, input.position.y / 300 / 2 - 1);

	float myDepth = input.vPorjPos.z;
	float4 fSceneDepth = gtxtSceneDepthTexture.Sample(gSamplerState, f2pos);

	float fSceenW = fSceneDepth.g * 600.f;
	float fSceenZ = fSceneDepth.r * fSceenW;


	float4 finalNoise = lerp(noise1, noise2, 0.7);
	float4 alphaColor = gtxtFinalAlpha.Sample(gSamplerClamp, input.tex);
	output.NonLight = float4(1, 1, 1, alphaColor.a * (finalNoise.a * 1.5));
	float fDepthDistance = fSceenZ - myDepth;
	if (/*(fDepthDistance < 80.f) &&*/ (fDepthDistance > 0))
	{
		fDepthDistance = saturate(1 - (fDepthDistance / 150.f)); // 0.7 //0.8 //0.9 
		//fDepthDistance *= 100; //0.3  //0.2 // 0.1 ..
		//fDepthDistance/= 
		output.NonLight.a -= fDepthDistance;
	}
	output.NonLight.a /= 7.5f;

	if (gf3RadiationLevel > 0)
	{
		float Percent = 1.f - float(gf3RadiationLevel) / 100.f;
		output.NonLight.r = output.NonLight.b = Percent;
		output.NonLight.g = 0.7f;
	}

	return output;

}
// [Special Fog] ==========================================================================

[maxvertexcount(4)]
void SpecialGS(point GS_NOISE_INPUT input[1], inout TriangleStream<PS_FOG_INPUT> outStream)
{
	float3 vUP = float3(0.0f, 1.0f, 0.0f);
	float3 vLook = angle;
	float3 vRight = cross(vUP, vLook);

	float fHalfW = 100.0f;
	float fHalfH = 70.0f;

	float4 pVertices[4];
	pVertices[0] = float4(input[0].positionW + fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[1] = float4(input[0].positionW + fHalfW * vRight + fHalfH * vUP, 1.0f);
	pVertices[2] = float4(input[0].positionW - fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[3] = float4(input[0].positionW - fHalfW * vRight + fHalfH * vUP, 1.0f);

	float2 pUVs[4] = { float2 (0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

	PS_FOG_INPUT output;

	output.position = mul(mul(pVertices[0], gmtxView), gmtxProjection);
	output.vPorjPos = output.position;
	output.tex = pUVs[0];

	output.tex1 = (output.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (output.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	outStream.Append(output);

	output.position = mul(mul(pVertices[1], gmtxView), gmtxProjection);
	output.vPorjPos = output.position;
	output.tex = pUVs[1];

	output.tex1 = (output.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (output.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	outStream.Append(output);

	output.position = mul(mul(pVertices[2], gmtxView), gmtxProjection);
	output.tex = pUVs[2];

	output.tex1 = (output.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (output.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);
	output.vPorjPos = output.position;

	outStream.Append(output);

	output.position = mul(mul(pVertices[3], gmtxView), gmtxProjection);
	output.vPorjPos = output.position;
	output.tex = pUVs[3];

	output.tex1 = (output.tex * scales.x);
	output.tex1.x = output.tex1.x + (frameTime * scrollSpeeds.x);

	output.tex2 = (output.tex * scales.x);
	output.tex2.x = output.tex2.x + (frameTime * scrollSpeeds.x);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	outStream.Append(output);


}
PS_NONLIGHT_MRT_OUTPUT SpecialFogPixelShader(PS_FOG_INPUT input)
{
	PS_NONLIGHT_MRT_OUTPUT output;

	float4 noise1 = gtxtAlpha01.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtAlpha02.Sample(gSamplerState, input.tex2);
	float2 f2pos = float2(input.position.x / 380 / 2 - 1, input.position.y / 300 / 2 - 1);

	float myDepth = input.vPorjPos.z;
	float4 fSceneDepth = gtxtSceneDepthTexture.Sample(gSamplerState, f2pos);

	float fSceenW = fSceneDepth.g * 600.f;
	float fSceenZ = fSceneDepth.r * fSceenW;


	float4 finalNoise = lerp(noise1, noise2, 1.0f);
	float4 alphaColor = gtxtFinalAlpha.Sample(gSamplerClamp, input.tex);
	output.NonLight = float4(0.8f, 0.8f, 0.8f, alphaColor.a * (finalNoise.a * 1.5));
	float fDepthDistance = fSceenZ - myDepth;
	if (/*(fDepthDistance < 80.f) &&*/ (fDepthDistance > 0))
	{
		fDepthDistance = saturate(1 - (fDepthDistance / 150.f)); // 0.7 //0.8 //0.9 
		//fDepthDistance *= 100; //0.3  //0.2 // 0.1 ..
		//fDepthDistance/= 
		output.NonLight.a -= fDepthDistance;
	}
	output.NonLight.a /= fogTime;

	if (gf3RadiationLevel > 0)
	{
		float Percent = 1.f - float(gf3RadiationLevel) / 100.f;
		output.NonLight.r = output.NonLight.b = Percent;
		output.NonLight.g = 0.8f;
	}

	return output;

}