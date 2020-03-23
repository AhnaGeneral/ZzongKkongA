#include "Shaders.hlsl"

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
	//float (input.vPosj.z / input.vProj.w , input,vProj.z / 500f,0,1) 
	float3 cColor = gtxtScene[int2(position.xy)].rgb;
	float fDepth = gtxtDepth[int2(position.xy)].r;
	float3 fLighted = gtxtLight[int2(position.xy)].rgb;

	cColor = lerp(cColor, fLighted, 0.6f);
	float3 cFogColor = float3(0.15f, 0.15f, 0.15f);
	cColor = lerp(cColor, cFogColor, fDepth * 5);

	return(float4(cColor, 1.0f));
}

static float gfLaplacians[9] = { -1.0f, -1.0f, -1.0f, -1.0f, 8.0f, -1.0f, -1.0f, -1.0f, -1.0f };
static int2 gnOffsets[9] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ -1,0 },{ 0,0 },{ 1,0 },{ -1,1 },{ 0,1 },{ 1,1 } };
//static int2 gnOffsets[9] = { {-1,-1}, {0,-1}, int2(1,-1), int2(-1,0), int2(0,0), int2(1,0), int2(-1,1), int2(0,1), int2(1,1) };


float4 PSPostProcessingByLaplacianEdge(float4 position : SV_POSITION) : SV_Target //backbufferm
{
	float fEdgeness = 0.0f;
	float fDepthEdgeness = 0.0f;
	float3 cEdgeness = float3(0.0f, 0.0f, 0.0f);
	if ((uint(position.x) >= 1) || (uint(position.y) >= 1) || (uint(position.x) <= gtxtNormal.Length.x - 2) || (uint(position.y) <= gtxtNormal.Length.y - 2))
	{
		for (int i = 0; i < 9; i++)
		{
			float3 vNormal = gtxtNormal[int2(position.xy) + gnOffsets[i]].xyz;
			vNormal = vNormal * 2.0f - 1.0f;
			cEdgeness += gfLaplacians[i] * vNormal;
		}
		fEdgeness = 1 - (cEdgeness.r * 0.3f + cEdgeness.g * 0.59f + cEdgeness.b * 0.11f);
		cEdgeness = float3(fEdgeness, fEdgeness, fEdgeness) ;
	}

	float3 cColor = gtxtScene[int2(position.xy)].rgb;
	

	cColor = (fEdgeness > 0.85f) ? cColor : ((fEdgeness > 0.35f) ? (cColor * cEdgeness) : cEdgeness);


	return(float4(cColor, 1.0f));

}


struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	//uint object:TEXCOORD1; 
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VSUI(VS_TEXTURED_INPUT input)  /// 직교투영
{
	VS_TEXTURED_OUTPUT output;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxOrthoView), gmtxOrtho);
	output.uv = input.uv;

	return(output);
}

float4 PSUI(VS_TEXTURED_OUTPUT input) :SV_TARGET //backbufferm
{
	float4 cColor = float4 (1.0f, 0.0f, 0.0f,0.0f);

	if (gnObjectID == 0)  cColor = gtxtNormal.Sample(gSamplerState, input.uv);
	if (gnObjectID == 1)  cColor = gtxtScene.Sample(gSamplerState, input.uv);
	if (gnObjectID == 2)  cColor = gtxtDepth.Sample(gSamplerState, input.uv);
	if (gnObjectID == 3)  cColor = gtxtLight.Sample(gSamplerState, input.uv);


	return cColor;
}