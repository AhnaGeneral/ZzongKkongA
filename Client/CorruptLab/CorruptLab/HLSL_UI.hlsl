#include "HLSL_MRT.hlsl"


//[PSHP]================================================================================
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


//[ITEM]================================================================================
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

//[Minimap]================================================================================
float4 PSMinimap(VS_TEXTURED_OUTPUT input) :SV_TARGET //backbuffer
{
	float4 Minmap = gtxtHandLighTexture.Sample(gSamplerState, input.uv);
	float4 Map_Fog1 = gtxtHPKitdTexture.Sample(gSamplerState, input.uv);
	float4 Map_Fog2 = gtxtPillddsTexture.Sample(gSamplerState, input.uv);

	if (Map_Fog1.a < 0.6f)
	{
		Map_Fog1.a = 0.0f;
	}

	if (Map_Fog2.a < 0.6f)
	{
		Map_Fog2.a = 0.0f;
	}

	float4 cColor = Minmap; /*+ Map_Fog2*/

	float2 playerpos = float2 (gf3PlayerPos.z / 512.0f, gf3PlayerPos.x / 512.0f);

	float fDistance = distance(playerpos, input.uv);

	if (fDistance < 0.03f)
		cColor = float4(1, 1, 0, 1);

	return cColor;
}

float4 PSPlayerHP(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float HP = (float)gfremainingHP / 100.0f;
    float4 cColor = gtxtRootUITexture.Sample(gSamplerClamp, input.uv);
	
	if (input.uv.x > HP)
		cColor = float4(1, 1, 1, 0);

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



//[MonsterHP]================================================================================

struct GS_BILLBOARD_OUTPUT
{
	float4 posH :SV_POSITION;
	float2 UV: TEXCOORD;
};


struct VS_BILLBOARD_INPUT
{
	float3 positionW : POSITION;
	float2 sizeW : SIZE;
};

struct VS_BILLBOARD_OUTPUT
{
	float3 positionW : POSITION;
	float2 sizeW : SIZE;
};

VS_BILLBOARD_INPUT VSMONSTERHP(VS_BILLBOARD_INPUT input)
{
	VS_BILLBOARD_OUTPUT output;

	//float4 PositionW = mul(float4(input.positionW, 1), gmtxGameObject);
	output.positionW.x = gmtxGameObject._41;
	output.positionW.y = gmtxGameObject._42 + (17 * input.sizeW.y);
	output.positionW.z = gmtxGameObject._43;

	output.sizeW = input.sizeW;
	return(output);
}


[maxvertexcount(4)]
void MosnterHPGS(point VS_BILLBOARD_OUTPUT input[1], inout TriangleStream<GS_BILLBOARD_OUTPUT> outStream)
{
	float3 vUP = float3(0.0f, 1.0f, 0.0f);
	float3 vLook = gvCameraPosition.xyz - input[0].positionW;
	vLook = normalize(vLook);
	float3 vRight = cross(vUP, vLook);

	float fHalfW = input[0].sizeW.x * 0.4f;
	float fHalfH = input[0].sizeW.y * 0.5f;

	float4 pVertices[4];
	pVertices[0] = float4(input[0].positionW + fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[1] = float4(input[0].positionW + fHalfW * vRight + fHalfH * vUP, 1.0f);
	pVertices[2] = float4(input[0].positionW - fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[3] = float4(input[0].positionW - fHalfW * vRight + fHalfH * vUP, 1.0f);

	float2 pUVs[4] = { float2 (0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

	GS_BILLBOARD_OUTPUT output;
	float Distance = distance(gvCameraPosition.xyz, input[0].positionW);
	if (Distance < 40){
		for (int i = 0; i < 4; ++i)
		{
			output.posH = mul(mul(pVertices[i], gmtxView), gmtxProjection);
			output.UV = pUVs[i];
			outStream.Append(output);
		}
	}
}



float4 PSMonsterHP(GS_BILLBOARD_OUTPUT input) : SV_TARGET4
{
	float HP = (float)gf3RadiationLevel / 100.0f;
	float4 cColor = gtxtAlbedoTexture.Sample(gSamplerClamp, input.UV);

	if (input.UV.x > HP)
		 cColor = float4(0.25f, 0.25f, 0.25f, 1);

	return float4(cColor);
}