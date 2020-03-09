
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
	//SetGraphicsRoot32BitConstants 함수 사용 할 때offset을 신경써주세요 
	matrix		    gmtxGameObject : packoffset(c0);
	uint            gnObjectID : packoffset (c4); 
    MATERIAL        gMaterial : packoffset(c5);
};

cbuffer cbOrthoInfo : register(b16)
{
	matrix		    gmtxOrtho: packoffset(c0);
	matrix		    gmtxOrthoView: packoffset(c4);
};

#include "Light.hlsl"

SamplerState gSamplerState : register(s0);

Texture2D<float4> gtxtScene : register(t1); // scene, normal, objectID RTV 0, 1, 2를 리소스 어레이로만든것
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
	float4 vPorjPos : TEXCOORD1; 
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
	output.vPorjPos = output.position;

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
	float4 object : SV_TARGET2; // 필요 없어보이니까 랜덤타겟 개수 확실히 정하고 바꿀때 지울것
};

PS_MULTIPLE_RENDER_TARGETS_OUTPUT PSTexturedLightingToMultipleRTs(VS_TEXTURED_LIGHTING_OUTPUT input)
{
	PS_MULTIPLE_RENDER_TARGETS_OUTPUT output;

	output.normal = float4(input.normalW, 1);
	float4 cColorAlbedo = gtxtAlbedoTexture.Sample(gSamplerState, input.uv);
	float4 cColorNormal = gtxtNormalTexture.Sample(gSamplerState, input.uv);

	float3 normalW;
	float3x3 TBN = float3x3(normalize(input.tangentW), normalize(input.bitangentW), normalize(input.normalW));
	float3 vNormal = normalize(cColorNormal.rgb * 2.0f - 1.0f); //[0, 1] → [-1, 1]
	normalW = normalize(mul(vNormal, TBN));

	float4 cColorLighted = Lighting(input.positionW, normalW);

	if (cColorAlbedo.a < 1.0f)
	{
		discard;
	}

	output.color = lerp(cColorAlbedo, cColorLighted, 0.4f);
	//output.color = float4(input.uv,1,1);
	return output;
}


