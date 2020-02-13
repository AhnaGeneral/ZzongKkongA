
struct MATERIAL
{
	float4		    m_cAmbient;
	float4		    m_cDiffuse;
	float4		    m_cSpecular; //a = power
	float4		    m_cEmissive;
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		    gmtxView : packoffset(c0);
	matrix		    gmtxProjection : packoffset(c4);
	float3		    gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		    gmtxGameObject : packoffset(c0);
	MATERIAL        gMaterial : packoffset(c4); 
};

#include "Light.hlsl"

SamplerState gSamplerState : register(s0);
Texture2D gtxtTexture : register(t0);

Texture2D<float4> gtxtScene : register(t1); // scene, normal, objectID RTV 0, 1, 2를 리소스 어레이로만든것

Texture2D<float4> gtxtNormal : register(t2);
Texture2D<float4> gtxtObject : register(t3);

struct VS_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	//	nointerpolation float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
};


VS_TEXTURED_LIGHTING_OUTPUT VSLighting(VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output; 
	
	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;
	return output; 
}

/////////////////////////////////////////////////////////////////////////////////////////////
struct PS_MULTIPLE_RENDER_TARGETS_OUTPUT
{
	float4 color  : SV_TARGET0; 
	float4 normal : SV_TARGET1;
	float4 object : SV_TARGET2; // 필요 없어보이니까 랜덤타겟 개수 확실히 정하고 바꿀때 지울것
};

PS_MULTIPLE_RENDER_TARGETS_OUTPUT PSTexturedLightingToMultipleRTs(VS_TEXTURED_LIGHTING_OUTPUT input)
{
	PS_MULTIPLE_RENDER_TARGETS_OUTPUT output; 
	
	output.normal = float4(input.normalW,1); 
	float4 cColorTex = gtxtTexture.Sample(gSamplerState, input.uv);
	float4 cColorLighted = Lighting(input.positionW, input.normalW);
	//output.color = lerp(cColorTex, cColorLighted, 0.6f); 
	output.color = cColorTex;
	return output;
}

///////////////////////////////////////////////////////////////////////////////
//
float4 VSPostProcessing(uint nVertexID : SV_VertexID) : SV_POSITION
{
	if (nVertexID == 0) return(float4(-1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 1) return(float4(+1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 2) return(float4(+1.0f, -1.0f, 0.0f, 1.0f));
	if (nVertexID == 3) return(float4(-1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 4) return(float4(+1.0f, -1.0f, 0.0f, 1.0f));
	if (nVertexID == 5) return(float4(-1.0f, -1.0f, 0.0f, 1.0f));

	return(float4(0, 0, 0, 0));
}

float4 PSPostProcessing(float4 position : SV_POSITION) : SV_Target
{
	float3 cColor = gtxtScene[int2(position.xy)].rgb;

	return(float4(cColor, 1.0f));
}

static float gfLaplacians[9] = { -1.0f, -1.0f, -1.0f, -1.0f, 8.0f, -1.0f, -1.0f, -1.0f, -1.0f };
static int2 gnOffsets[9] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ -1,0 },{ 0,0 },{ 1,0 },{ -1,1 },{ 0,1 },{ 1,1 } };
//static int2 gnOffsets[9] = { {-1,-1}, {0,-1}, int2(1,-1), int2(-1,0), int2(0,0), int2(1,0), int2(-1,1), int2(0,1), int2(1,1) };


float4 PSPostProcessingByLaplacianEdge(float4 position : SV_POSITION) : SV_Target //backbufferm
{
	float fEdgeness = 0.0f;
	float3 cEdgeness = float3(0.0f, 0.0f, 0.0f);
	if ((uint(position.x) >= 1) || (uint(position.y) >= 1) || (uint(position.x) <= gtxtNormal.Length.x - 2) || (uint(position.y) <= gtxtNormal.Length.y - 2))
	{
		float fObject = 0.0f;
		for (int i = 0; i < 9; i++)
		{
			float3 vNormal = gtxtNormal[int2(position.xy) + gnOffsets[i]].xyz;
			vNormal = vNormal * 2.0f - 1.0f;
			cEdgeness += gfLaplacians[i] * vNormal;
			fObject += gtxtObject[int2(position.xy) + gnOffsets[i]].r * (1.0f / 9.0f);
		}
		fEdgeness = cEdgeness.r * 0.3f + cEdgeness.g * 0.59f + cEdgeness.b * 0.11f;
		if ((fEdgeness < 0.15f) && (abs(fObject - gtxtObject[int2(position.xy)].r) > 0.01f)) fEdgeness = 1.0f;
		cEdgeness = float3(fEdgeness, fEdgeness, fEdgeness);
	}
	float3 cColor = gtxtScene[int2(position.xy)].rgb;
	cColor = (fEdgeness < 0.15f) ? cColor : ((fEdgeness < 0.65f) ? (cColor + cEdgeness) : cEdgeness);

	return(float4(cColor, 1.0f));
}
