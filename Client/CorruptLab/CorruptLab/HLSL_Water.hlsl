#include "Shaders.hlsl"


Texture2D<float4> gtxtWaterNormal : register(t24);


struct VS_WATER_INPUT
{
	float3 positionW : POSITION;
	float2 sizeW : SIZE;
};

struct VS_WATER_OUTPUT
{
	float3 centerW : POSITION;
	float2 sizeW : SIZE;
};

struct GS_WATER_OUTPUT
{
	float4 posH :SV_POSITION;
	float3 posW :POSITION;
	float2 UV: TEXCOORD;
};


VS_WATER_OUTPUT VSWater(VS_WATER_INPUT input)
{
	VS_WATER_OUTPUT output;

	output.centerW = input.positionW;
	output.sizeW = input.sizeW;
	return(output);
}

[maxvertexcount(4)]
void GS(point VS_WATER_OUTPUT input[1], inout TriangleStream<GS_WATER_OUTPUT> outStream)
{
	float3 vUP = float3(0.0f, 0.0f, 1.0f);
	float3 vRight = float3(-1.0f, 0.0f, 0.0f);

	float fHalfW = input[0].sizeW.x;
	float fHalfH = input[0].sizeW.y;

	float4 pVertices[4];

	pVertices[0] = float4(input[0].centerW + fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[1] = float4(input[0].centerW + fHalfW * vRight + fHalfH * vUP, 1.0f);
	pVertices[2] = float4(input[0].centerW - fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[3] = float4(input[0].centerW - fHalfW * vRight + fHalfH * vUP, 1.0f);



	float2 pUVs[4] = { float2 (0.0f, 500.0f), float2(0.0f, 0.0f), float2(500.0f, 500.0f), float2(500.0f, 0.0f) };

	GS_WATER_OUTPUT output;

	for (int i = 0; i < 4; ++i)
	{
		output.posW = pVertices[i].xyz;
		output.posH = mul(mul(pVertices[i], gmtxView), gmtxProjection);
		output.UV = pUVs[i];
		outStream.Append(output);
	}
}

PS_MULTIPLE_RENDER_TARGETS_OUTPUT PSWater(GS_WATER_OUTPUT input)  : SV_TARGET
{

	PS_MULTIPLE_RENDER_TARGETS_OUTPUT output;
	
	float4 normal = gtxtWaterNormal.Sample(gSamplerState, input.uv);
	float3 tangent = float3(0, 0, -1);
	normal = float4(cross(normal.xyz, tangent),0.6f);

	output.normal = normal;

	float fDepth = input.posH.z / 1000.f;
	output.color = float4(0, 0.3f, 0.4f, 0.8f);
	output.depth = float4(fDepth, fDepth, fDepth, 1.0f);

	return(output);
}