
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
	matrix			gmtxInverseView : packoffset(c8);
	matrix			gmtxInverseProjection : packoffset(c12);
	float3		    gvCameraPosition : packoffset(c16);
};


cbuffer cbGameObjectInfo : register(b2)
{
	matrix		    gmtxGameObject : packoffset(c0);
	uint            gnObjectID : packoffset (c4); 
};

cbuffer cbOrthoInfo : register(b16)
{
	matrix		    gmtxOrtho: packoffset(c0);
	matrix		    gmtxOrthoView: packoffset(c4);
};


cbuffer cbShadowCameraInfo : register(b5)
{
	matrix		    shadowgmtxView : packoffset(c0);
	matrix		    shadowgmtxProjection : packoffset(c4);
	matrix			shadowgmtxInverseView : packoffset(c8);
	matrix			shadowgmtxInverseProjection : packoffset(c12);
	float3		    shadowgvCameraPosition : packoffset(c16);
};

#include "Light.hlsl"

SamplerState gSamplerState : register(s0);
SamplerState gSamplerClamp : register(s1);

Texture2D gtxtScene : register(t1);  // 0  
Texture2D gtxtNormal : register(t2); // 1
Texture2D gtxtDepth : register(t3);  // 2 
Texture2D gtxtShadowCameraViewDepth : register(t4); // 3
Texture2D gtxtNonLightNoise : register(t5); // 4 


Texture2D gtxtLight : register(t20);

Texture2D gtxtAlbedoTexture : register(t6);
Texture2D gtxtSpecularTexture : register(t7);
Texture2D gtxtNormalTexture : register(t8);
Texture2D gtxtMetallicTexture : register(t9);
Texture2D gtxtEmissionTexture : register(t10);

TextureCube gtxtSkyCubeTexture : register(t11);
Texture2D gtxCloudTextures : register(t12);

Texture2D gtxtBaseColorNoiseTex : register(t13);
Texture2D gtxtAlphaNoiseTex : register(t14);
Texture2D gtxtNoiseTex : register(t15);

Texture2D gtxtFinalAlpha : register(t16);
Texture2D gtxtAlpha01 : register(t17);
Texture2D gtxtAlpha02 : register(t18);

Texture2D gtxtWaterNormal : register(t19);


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
	output.vPorjPos = output.position;
	return output;
}

/////////////////////////////////////////////////////////////////////////////////////////////
struct PS_MULTIPLE_RENDER_TARGETS_OUTPUT
{
	float4 color        : SV_TARGET0;
	float4 normal       : SV_TARGET1;
	float4 depth        : SV_TARGET2;
	float4 ShadowCamera : SV_TARGET3; 
};

struct PS_NONLIGHT_MRT_OUTPUT
{
	float4 NonLight : SV_TARGET4;
};


PS_MULTIPLE_RENDER_TARGETS_OUTPUT PSTexturedLightingToMultipleRTs(VS_TEXTURED_LIGHTING_OUTPUT input)
{
	PS_MULTIPLE_RENDER_TARGETS_OUTPUT output;

	float4 cColorAlbedo = gtxtAlbedoTexture.Sample(gSamplerState, input.uv);
	float4 cColorNormal = gtxtNormalTexture.Sample(gSamplerState, input.uv);

	cColorAlbedo.rgb = cColorAlbedo.rgb;

	float3 normalW;
	float3x3 TBN = float3x3(normalize(input.tangentW), normalize(input.bitangentW), normalize(input.normalW));

	float3 vNormal = normalize(cColorNormal.rgb * 2.0f - 1.0f); //[0, 1] ¡æ [-1, 1]

	normalW = normalize(mul(vNormal, TBN));

	float fdepth = input.vPorjPos.z / 1000.f;
	output.normal = float4(normalW, 1);
	output.color = cColorAlbedo;
	output.depth = float4(input.positionW, 1);
	output.ShadowCamera = float4 (1.0f, 0.0f, 0.0f, 1.0f);
	//output.NonLight = float4 (1.0f, 0.0f, 0.0f, 1.0f);

	return output;
}


struct VS_STANDARD_SHADOWOUTPUT
{
	float3 positionW : POSITION;
	float4 position : SV_POSITION;
};

VS_STANDARD_SHADOWOUTPUT VSStandardShadow (VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_STANDARD_SHADOWOUTPUT output;
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	return output;
}

VS_STANDARD_SHADOWOUTPUT VSSkinnedShadow(VS_SKINNED_STANDARD_INPUT input)
{
	VS_STANDARD_SHADOWOUTPUT output;

	output.positionW = float3(0.0f, 0.0f, 0.0f);
	matrix mtxVertexToBoneWorld;
	for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
	{
		mtxVertexToBoneWorld = mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
		output.positionW += input.weights[i] * mul(float4(input.position, 1.0f), mtxVertexToBoneWorld).xyz;
	}

	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	return(output);
}