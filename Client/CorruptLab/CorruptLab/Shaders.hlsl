
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

Texture2D<float4> gtxtScene : register(t1); // scene, normal, objectID RTV 0, 1, 2�� ���ҽ� ��̷θ����
Texture2D<float4> gtxtNormal : register(t2);
Texture2D<float4> gtxtObject : register(t3);

Texture2D gtxtAlbedoTexture : register(t4);
Texture2D gtxtSpecularTexture : register(t5);
Texture2D gtxtNormalTexture : register(t6);
Texture2D gtxtMetallicTexture : register(t7);
Texture2D gtxtEmissionTexture : register(t8);
Texture2D gtxtDetailAlbedoTexture : register(t9);
Texture2D gtxtDetailNormalTexture : register(t10);

struct VS_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
	float2 uv : TEXCOORD;
};

struct VS_SKINNED_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	uint4 indices : BONEINDEX;
	float4 weights : BONEWEIGHT;
};

#define MAX_VERTEX_INFLUENCES			4
#define SKINNED_ANIMATION_BONES			128

cbuffer cbBoneOffsets : register(b7)
{
	float4x4 gpmtxBoneOffsets[SKINNED_ANIMATION_BONES];
};

cbuffer cbBoneTransforms : register(b8)
{
	float4x4 gpmtxBoneTransforms[SKINNED_ANIMATION_BONES];
};

VS_TEXTURED_LIGHTING_OUTPUT VSSkinnedAnimationStandard(VS_SKINNED_STANDARD_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output;

	output.positionW = float3(0.0f, 0.0f, 0.0f);
	output.normalW = float3(0.0f, 0.0f, 0.0f);
	output.tangentW = float3(0.0f, 0.0f, 0.0f);
	output.bitangentW = float3(0.0f, 0.0f, 0.0f);
	matrix mtxVertexToBoneWorld;
	for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
	{
		mtxVertexToBoneWorld = mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
		output.positionW += input.weights[i] * mul(float4(input.position, 1.0f), mtxVertexToBoneWorld).xyz;
		output.normalW += input.weights[i] * mul(input.normal, (float3x3)mtxVertexToBoneWorld);
		output.tangentW += input.weights[i] * mul(input.tangent, (float3x3)mtxVertexToBoneWorld);
		output.bitangentW += input.weights[i] * mul(input.bitangent, (float3x3)mtxVertexToBoneWorld);
	}

	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

VS_TEXTURED_LIGHTING_OUTPUT VSLighting(VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output; 
	
	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.tangentW = mul(input.tangent, (float3x3)gmtxGameObject);
	output.bitangentW = mul(input.bitangent, (float3x3)gmtxGameObject);
	output.uv = input.uv;
	return output; 
}

/////////////////////////////////////////////////////////////////////////////////////////////
struct PS_MULTIPLE_RENDER_TARGETS_OUTPUT
{
	float4 color  : SV_TARGET0; 
	float4 normal : SV_TARGET1;
	float4 object : SV_TARGET2; // �ʿ� ����̴ϱ� ����Ÿ�� ���� Ȯ���� ���ϰ� �ٲܶ� �����
};

PS_MULTIPLE_RENDER_TARGETS_OUTPUT PSTexturedLightingToMultipleRTs(VS_TEXTURED_LIGHTING_OUTPUT input)
{
	PS_MULTIPLE_RENDER_TARGETS_OUTPUT output; 
	
	output.normal = float4(input.normalW,1); 
	float4 cColorAlbedo = gtxtAlbedoTexture.Sample(gSamplerState, input.uv);
	float4 cColorNormal = gtxtNormalTexture.Sample(gSamplerState, input.uv);

	float4 cColorLighted = Lighting(input.positionW, input.normalW);
	output.color = lerp(cColorAlbedo, cColorLighted, 0.6f);
	//output.color = float4(input.uv,1,1);
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
