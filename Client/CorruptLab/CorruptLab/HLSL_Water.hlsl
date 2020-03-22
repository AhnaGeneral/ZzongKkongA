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

	float fHalfW = input[0].sizeW.x * 0.5f;
	float fHalfH = input[0].sizeW.y * 0.5f;

	float4 pVertices[4];

	pVertices[0] = float4(input[0].centerW.x - fHalfW, input[0].centerW.y, input[0].centerW.z - fHalfH, 1.0f);
	pVertices[1] = float4(input[0].centerW.x - fHalfW, input[0].centerW.y, input[0].centerW.z + fHalfH, 1.0f);
	pVertices[2] = float4(input[0].centerW.x + fHalfW, input[0].centerW.y, input[0].centerW.z - fHalfH, 1.0f);
	pVertices[3] = float4(input[0].centerW.x + fHalfW, input[0].centerW.y, input[0].centerW.z + fHalfH, 1.0f);

	float2 pUVs[4] = { float2 (0.0f, 20.0f), float2(0.0f, 0.0f), float2(20.0f, 20.0f), float2(20.0f, 0.0f) };

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

	float3 uvw = float3 (input.UV,0.0f);
	
	float4 normal = gtxtWaterNormal.Sample(gSamplerState, uvw);
	float3 tangent = float3(0, 0, -1);
	normal = float4(cross(normal.xyz, tangent),0.6f);

	output.normal = normal;

	float fDepth = input.posH.z / 1000.f;
	output.color = float4(0, 0, 1, 0.4f);
	output.depth = float4(fDepth, fDepth, fDepth, 1.0f);

	return(output);
}