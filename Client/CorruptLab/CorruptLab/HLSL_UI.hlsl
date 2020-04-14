#include "HLSL_MRT.hlsl"

float4 PSMinimap(VS_TEXTURED_OUTPUT input) :SV_TARGET //backbuffer
{
	float4 cColor = gtxtLight.Sample(gSamplerState, input.uv);

	float2 playerpos = float2 (gf3PlayerPos.z / 512.0f, gf3PlayerPos.x / 512.0f );
	
	float fDistance  =  distance(playerpos, input.uv);

	if (fDistance < 0.02f)
		cColor = float4(1, 0, 0, 1);

	return cColor;
}