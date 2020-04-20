#include "HLSL_MRT.hlsl"

float4 PSMinimap(VS_TEXTURED_OUTPUT input) :SV_TARGET //backbuffer
{
	float4 cColor = gtxtLight.Sample(gSamplerState, input.uv);

	float2 playerpos = float2 (gf3PlayerPos.z / 512.0f, gf3PlayerPos.x / 512.0f );
	
	float fDistance  =  distance(playerpos, input.uv);

	if (fDistance < 0.03f)
		cColor = float4(1, 1, 0, 1);

	return cColor;
}

Texture2D gtxtRootUITexture : register(t25);


float4 PSHP(VS_TEXTURED_OUTPUT input ): SV_TARGET
{
	float4 cColor = gtxtRootUITexture.Sample(gSamplerClamp, input.uv);
	
	if (cColor.a > 0.6f)
		cColor.a = 1.0f;

	return float4(cColor);
}