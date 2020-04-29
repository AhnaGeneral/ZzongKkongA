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

cbuffer cbPlayerHPRemaining : register(b8) // 플레이어
{
	uint          gfremainingHP :packoffset(c0);
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



float4 PSPlayerHP(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtRootUITexture.Sample(gSamplerClamp, input.uv);

	if (cColor.a > 0.6f)
		cColor.a = 1.0f;

	float HP = gfremainingHP / 100.0f;
	//cColor.r = cColor.r * HP; 

	return float4(cColor);
}


VS_TEXTURED_OUTPUT BillboardUI_VS(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	float3 positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;
	return output;
}


PS_NONLIGHT_MRT_OUTPUT BillboardUI_PS(VS_TEXTURED_OUTPUT input)
{

	PS_NONLIGHT_MRT_OUTPUT output;

	output.NonLight = gtxtAlbedoTexture.Sample(gSamplerState, input.uv);
	//output.NonLight = float4(1, 1, 1, 1);
	return output;
}

float4 PSRadiationLevel(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float2 uv = input.uv;
	uv.x /= 11.f;
	uv.x += (1.f / 11.f) * (gfremainingHP);
	
	float4 cColor = gtxtRootUITexture.Sample(gSamplerClamp, uv);

	if (cColor.a > 0.6f)
		cColor.a = 1.0f;

	return float4(cColor);
}