#include "Shaders.hlsl"

Texture2D gtxtTerrainBaseTexture : register(t13);
Texture2D gtxtTerrainAlaphTexture : register(t14);
Texture2D gtxtTerrainNormalTexture : register(t15);



struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	//float4 posj : TEXCOORD2;
};

struct VS_TERRAIN_OUTPUT
{
	float3 position : POSITION;
	float4 color : COLOR;

	float3 normal : NORMAL;
	//float3 tangent : TANGET ;
	//float3 bitanget : BITANGENT; 

	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float4 posj : TEXCOORD2;

};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

	output.color = input.color;
	output.position = input.position;

	//float3 worldnormal = float3(1.0f, 1.0f, 1.0f); 
	//float3 worldtanget = float3(1.0f, 1.0f, 1.0f);
	//float3 worldbitanget = float3(1.0f, 1.0f, 1.0f);


	//worldnormal = mul(input.normal, (float3x3)gmtxGameObject);
	//output.normal = normalize(worldnormal);

	//float3 tmptanget = cross(input.normal, float3(0.0f, 1.0f, 0.0f)); 
	//worldtanget = mul(tmptanget, (float3x3)gmtxGameObject);
	//output.tangent = normalize(worldtanget);

	//float3 tmpbitanget = cross(input.normal, tmptanget);
	//worldbitanget = mul(tmpbitanget, (float3x3)gmtxGameObject);

	//output.bitanget = normalize(worldbitanget);
	
	output.normal = input.normal; 

	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	output.posj = float4 (input.position, 1.0f);
	return(output);
}

struct HS_TERRAIN_TESSELLATION_CONSTANT
{
	float fTessEdges[4] : SV_TessFactor;
	float fTessInsides[2] : SV_InsideTessFactor;
};

struct HS_TERRAIN_TESSELLATION_OUTPUT
{
	float3 position : POSITION;
	float4 color : COLOR;

	float3 normal : NORMAL;
	//float3 tangent : TANGET;
	//float3 bitanget : BITANGENT;

	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float4 posj : TEXCOORD2;
};

struct DS_TERRAIN_TESSELLATION_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;

	float3 normal : NORMAL;
	float3 tangent : TANGET;
	float3 bitanget : BITANGENT;

	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float4 posj : TEXCOORD2;
};

void BernsteinCoeffcient5x5(float t, out float fBernstein[5])
{
	float tInv = 1.0f - t;
	fBernstein[0] = tInv * tInv * tInv * tInv;
	fBernstein[1] = 4.0f * t * tInv * tInv * tInv;
	fBernstein[2] = 6.0f * t * t * tInv * tInv;
	fBernstein[3] = 4.0f * t * t * t * tInv;
	fBernstein[4] = t * t * t * t;
}

float3 CubicBezierSum5x5(OutputPatch<HS_TERRAIN_TESSELLATION_OUTPUT, 25> patch, float uB[5], float vB[5])
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
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(25)]
[patchconstantfunc("VSTerrainTessellationConstant")]
[maxtessfactor(64.0f)]
HS_TERRAIN_TESSELLATION_OUTPUT HSTerrainTessellation(InputPatch<VS_TERRAIN_OUTPUT, 25> input, uint i : SV_OutputControlPointID)
{
	HS_TERRAIN_TESSELLATION_OUTPUT output;

	output.position = input[i].position;
	output.color = input[i].color;
	output.uv0 = input[i].uv0;
	output.uv1 = input[i].uv1;
	output.normal = input[i].normal; 

	//output.tangent = input[i].tangent;
	//output.bitanget = input[i].bitanget;

	output.posj = input[i].posj;

	return(output);
}

HS_TERRAIN_TESSELLATION_CONSTANT VSTerrainTessellationConstant(InputPatch<VS_TERRAIN_OUTPUT, 25> input)
{

	float3 vCenter;
	for (int i = 0; i < 25; i++)
		vCenter += input[i].position;
	vCenter = vCenter / 25.f;

	float fDistanceToCamera = distance(vCenter, gvCameraPosition);

	float fTessFactor = (1/ fDistanceToCamera) * 150;
	//if (fDistanceToCamera > 1000.f) fTessFactor = 1.f;
	HS_TERRAIN_TESSELLATION_CONSTANT output;

	//output.fTessEdges[0] = fTessFactor;
	//output.fTessEdges[1] = fTessFactor;
	//output.fTessEdges[2] = fTessFactor;
	//output.fTessEdges[3] = fTessFactor;
	//output.fTessInsides[0] = fTessFactor;
	//output.fTessInsides[1] = fTessFactor;

	// 일정한 격자로 보고 싶을 때 

	output.fTessEdges[0] = 3.0f;
	output.fTessEdges[1] = 3.0f;
	output.fTessEdges[2] = 3.0f;
	output.fTessEdges[3] = 3.0f;
	output.fTessInsides[0] = 3.0f;
	output.fTessInsides[1] = 3.0f;

	return(output);
}

[domain("quad")]
[earlydepthstencil]
DS_TERRAIN_TESSELLATION_OUTPUT DSTerrainTessellation(HS_TERRAIN_TESSELLATION_CONSTANT patchConstant, float2 uv : SV_DomainLocation, OutputPatch<HS_TERRAIN_TESSELLATION_OUTPUT, 25> patch)
{
	DS_TERRAIN_TESSELLATION_OUTPUT output = (DS_TERRAIN_TESSELLATION_OUTPUT)0;

	float uB[5], vB[5];
	BernsteinCoeffcient5x5(uv.x, uB);
	BernsteinCoeffcient5x5(uv.y, vB);

	output.color = lerp(lerp(patch[0].color, patch[4].color, uv.x), lerp(patch[20].color, patch[24].color, uv.x), uv.y);
	output.uv0 = lerp(lerp(patch[0].uv0, patch[4].uv0, uv.x), lerp(patch[20].uv0, patch[24].uv0, uv.x), uv.y);
	output.uv1 = lerp(lerp(patch[0].uv1, patch[4].uv1, uv.x), lerp(patch[20].uv1, patch[24].uv1, uv.x), uv.y);
	float3 worldnormal = lerp(lerp(patch[0].normal, patch[4].normal, uv.x), lerp(patch[20].normal, patch[24].normal, uv.x), uv.y);
	
	///float3 tangentuv = float3(output.uv0, 0.0f); //텍스처의 UV값을  

    float3 worldtanget = float3(1.0f, 1.0f, 1.0f);
    float3 worldbitanget = float3(1.0f, 1.0f, 1.0f);

    float3 tmpnormal = mul(worldnormal, (float3x3)gmtxGameObject);
    output.normal = normalize(tmpnormal);

    float3 tmptanget = cross(tmpnormal, float3(output.uv0.x, 0.0f, 0.0f));
    worldtanget = mul(tmptanget, (float3x3)gmtxGameObject);
    output.tangent = normalize(worldtanget);
    
    float3 tmpbitanget = cross(tmpnormal, tmptanget);
    worldbitanget = mul(tmpbitanget, (float3x3)gmtxGameObject);
    output.bitanget = worldbitanget;

	float3 position = CubicBezierSum5x5(patch, uB, vB);
	matrix mtxWorldViewProjection = mul(mul(gmtxGameObject, gmtxView), gmtxProjection);
	output.position = mul(float4(position, 1.0f), mtxWorldViewProjection);


	output.posj = output.position;
	return(output);
}


Texture2D gtxtStone1_BC : register(t16);
Texture2D gtxtGrass2_BC : register(t17);
Texture2D gtxtDryStone_BC : register(t18);

//Texture2D gtxDryGround_BC : register(t19);
//Texture2D gtxGrass1_BC : register(t20);
//Texture2D gtxGrass2_BC : register(t21);

PS_MULTIPLE_RENDER_TARGETS_OUTPUT PSTerrain(DS_TERRAIN_TESSELLATION_OUTPUT input) : SV_TARGET
{
	PS_MULTIPLE_RENDER_TARGETS_OUTPUT output; 
	//float4 cBaseTexColor = float4(0.2f,0.2f,0.2f,1.f);

	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv0);
	float4 cDetailTexColor = gtxtTerrainAlaphTexture.Sample(gSamplerState, input.uv0);
	float4 cNormalTexColor = gtxtTerrainNormalTexture.Sample(gSamplerState, input.uv0);

	float4 CStone01 = gtxtStone1_BC.Sample(gSamplerState, input.uv1);
	float4 CDryStone = gtxtGrass2_BC.Sample(gSamplerState, input.uv1);
	float4 CGrass2_BC = gtxtGrass2_BC.Sample(gSamplerState, input.uv1);

	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (cDetailTexColor.r)
		cColor = saturate((cBaseTexColor * 0.5f) + (CStone01 * 0.5f));
	else if (cDetailTexColor.g)
		cColor = saturate((cBaseTexColor * 0.5f) + (CDryStone * 0.5f));
	else if (cDetailTexColor.b)
		cColor = saturate((cBaseTexColor * 0.5f) + (CGrass2_BC * 0.5f));
	else 
		cColor = (cBaseTexColor * 1.0f);

	float3 ncColor = cColor.rgb;
	output.color = float4 (ncColor, 1.0f);

	float3x3 TBN = float3x3(input.tangent, input.bitanget, input.normal);
	float3 vNormal = normalize(cNormalTexColor.rgb * 2.0f - 1.0f);//0.0 ~ 1.0 -> -1.0 ~ 1.0 
	vNormal = normalize(mul(vNormal, TBN));

	output.normal = float4(vNormal, 1.0f);

	output.depth = float4(input.posj.z / 1000.f, input.posj.z / 1000.f, input.posj.z / 1000.f, 1.0f);


	return output;
}