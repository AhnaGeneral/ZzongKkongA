
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

cbuffer cbOrthoInfo : register(b4)
{
	matrix		    gmtxOrtho: packoffset(c0);
	matrix		    gmtxOrthoView: packoffset(c4);
};

cbuffer cbShadowCameraInfo : register(b5) // 그림자 카메라 
{
	matrix		    shadowgmtxView : packoffset(c0);
	matrix		    shadowgmtxProjection : packoffset(c4);
	matrix			shadowgmtxInverseView : packoffset(c8);
	matrix			shadowgmtxInverseProjection : packoffset(c12);
	float3		    shadowgvCameraPosition : packoffset(c16);
};


cbuffer cbPlayerPosition : register(b6) // 플레이어 위치
{
	float3			gf3PlayerPos :packoffset(c0);
}

#include "Light.hlsl"

SamplerState gSamplerState : register(s0);
SamplerState gSamplerClamp : register(s1);

Texture2D gtxtScene : register(t1);  // 0  
Texture2D gtxtNormal : register(t2); // 1
Texture2D gtxtDepth : register(t3);  // 2 
//T4번이 빈다.

Texture2D gtxtNonLightNoise : register(t5); // 4 


Texture2D gtxtLight : register(t20);
Texture2D gtxtShadowCameraTexture : register(t21);

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
Texture2D gtxtCopySunTex : register(t23); // 썬카메라를 새롭게 리소스 만들었어요


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

struct PS_SHADOW_OUTPUT
{
	float4 ShadowTex : SV_TARGET0;
};

static int2 gnOffsets[9] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ -1,0 },{ 0,0 },{ 1,0 },{ -1,1 },{ 0,1 },{ 1,1 } };
