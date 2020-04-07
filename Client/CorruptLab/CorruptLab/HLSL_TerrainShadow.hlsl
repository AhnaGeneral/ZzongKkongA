#include "HLSL_Terrain.hlsl"


//============================================================================================================================


struct VS_TERRAIN_SHADOW_OUTPUT
{
	float3 position : POSITION;
	float4 posj : TEXCOORD2;
};

struct HS_TERRAIN_TESSELLATION_SHADOW_OUTPUT
{
	float3 position : POSITION;
	float4 posj : TEXCOORD2;
};

struct DS_TERRAIN_TESSELLATION_SHADOW_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
};


VS_TERRAIN_SHADOW_OUTPUT VSTerrainShadow(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_SHADOW_OUTPUT output;

	output.position = input.position;
	output.posj = float4 (input.position, 1.0f);

	return(output);
}


HS_TERRAIN_TESSELLATION_CONSTANT VSTerrainTessellationConstantShadow(InputPatch<VS_TERRAIN_SHADOW_OUTPUT, 25> input)
{

	HS_TERRAIN_TESSELLATION_CONSTANT output;

	output.fTessEdges[0] = 3.0f;
	output.fTessEdges[1] = 3.0f;
	output.fTessEdges[2] = 3.0f;
	output.fTessEdges[3] = 3.0f;
	output.fTessInsides[0] = 3.0f;
	output.fTessInsides[1] = 3.0f;

	return(output);
}



[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(25)]
[patchconstantfunc("VSTerrainTessellationConstantShadow")]
[maxtessfactor(64.0f)]
HS_TERRAIN_TESSELLATION_SHADOW_OUTPUT HSTerrainTessellationShadow(InputPatch<VS_TERRAIN_SHADOW_OUTPUT, 25> input, uint i : SV_OutputControlPointID)
{
	HS_TERRAIN_TESSELLATION_SHADOW_OUTPUT output;

	output.position = input[i].position;
	output.posj = input[i].posj;

	return(output);
}


float3 CubicBezierSum5x5(OutputPatch<HS_TERRAIN_TESSELLATION_SHADOW_OUTPUT, 25> patch, float uB[5], float vB[5])
{
	float3 f3Sum = float3(0.0f, 0.0f, 0.0f);
	f3Sum = vB[0] * (uB[0] * patch[0].position + uB[1] * patch[1].position + uB[2] * patch[2].position + uB[3] * patch[3].position + uB[4] * patch[4].position);
	f3Sum += vB[1] * (uB[0] * patch[5].position + uB[1] * patch[6].position + uB[2] * patch[7].position + uB[3] * patch[8].position + uB[4] * patch[9].position);
	f3Sum += vB[2] * (uB[0] * patch[10].position + uB[1] * patch[11].position + uB[2] * patch[12].position + uB[3] * patch[13].position + uB[4] * patch[14].position);
	f3Sum += vB[3] * (uB[0] * patch[15].position + uB[1] * patch[16].position + uB[2] * patch[17].position + uB[3] * patch[18].position + uB[4] * patch[19].position);
	f3Sum += vB[4] * (uB[0] * patch[20].position + uB[1] * patch[21].position + uB[2] * patch[22].position + uB[3] * patch[23].position + uB[4] * patch[24].position);
	return(f3Sum);
}

[domain("quad")]
[earlydepthstencil]
DS_TERRAIN_TESSELLATION_SHADOW_OUTPUT DSTerrainTessellationShadow(HS_TERRAIN_TESSELLATION_CONSTANT patchConstant, float2 uv : SV_DomainLocation, OutputPatch<HS_TERRAIN_TESSELLATION_SHADOW_OUTPUT, 25> patch)
{
	DS_TERRAIN_TESSELLATION_SHADOW_OUTPUT output = (DS_TERRAIN_TESSELLATION_SHADOW_OUTPUT)0;

	float uB[5], vB[5];
	BernsteinCoeffcient5x5(uv.x, uB);
	BernsteinCoeffcient5x5(uv.y, vB);

	///float3 tangentuv = float3(output.uv0, 0.0f); //텍스처의 UV값을  

	float3 position = CubicBezierSum5x5(patch, uB, vB);
	matrix mtxWorldViewProjection = mul(mul(gmtxGameObject, shadowgmtxView), shadowgmtxProjection);

	//matrix
	output.positionW = mul(float4(position, 1.0f), gmtxGameObject);
	output.position = mul(float4(position, 1.0f), mtxWorldViewProjection);

	return(output);
}


PS_SHADOW_OUTPUT PSTerrainShadow(DS_TERRAIN_TESSELLATION_SHADOW_OUTPUT input)
{
	PS_SHADOW_OUTPUT output;

	output.ShadowTex = float4(input.position.z / 300.f, input.position.w / 300.f, 0.0f, 1.0f);
	//output.ShadowTex = float4(input.position.z  , 0, 0.0f, 1.0f);
	return output;
}
