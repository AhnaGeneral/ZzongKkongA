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

cbuffer cbItemReaction : register(b7) // 플레이어 위치
{
	uint	   gf3ItemReaction :packoffset(c0);
}

Texture2D gtxtHandLighTexture : register(t26);
Texture2D gtxtHPKitdTexture : register(t27);
Texture2D gtxtPillddsTexture : register(t28);


float4 PSItem(VS_TEXTURED_OUTPUT input) : SV_TARGET
{

	float4 cHandLight = gtxtHandLighTexture.Sample(gSamplerClamp, input.uv);
	float4 cHPKit = gtxtHPKitdTexture.Sample(gSamplerClamp, input.uv);
	float4 cPillds = gtxtPillddsTexture.Sample(gSamplerClamp, input.uv);

	float4 cColor;

	if (gf3ItemReaction == 3)
	{
		if (gnObjectID == 0) cColor = cHandLight;
		if (gnObjectID == 1) cColor = cHPKit;
		if (gnObjectID == 2) cColor = cPillds;
	}
	else
	{
		if (gnObjectID == 0 && gf3ItemReaction == 0)
		{
			cColor = float4(cHandLight.r + 0.4, cHandLight.gba);
		}

		else if (gnObjectID == 1 && gf3ItemReaction == 1)
		{
			cColor = float4(cHPKit.r + 0.4, cHPKit.gba);

		}

		else if (gnObjectID == 2 && gf3ItemReaction == 2)
		{
			cColor = float4(cPillds.r + 0.4, cPillds.gba);
		}

		else if (gnObjectID == 0) cColor = cHandLight;
		else if (gnObjectID == 1) cColor = cHPKit;
		else if (gnObjectID == 2) cColor = cPillds;
	}

	if (cColor.a > 0.6f)
		cColor.a = 1.0f;

	return float4(cColor);
}