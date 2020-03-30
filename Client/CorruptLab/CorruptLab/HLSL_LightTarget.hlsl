#include "Shaders.hlsl"

struct PS_LRT_OUTPUT
{
	float4 Light : SV_TARGET0;
};

float4 VSLightTarget(uint nVertexID : SV_VertexID) : SV_POSITION
{
	if (nVertexID == 0) return(float4(-1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 1) return(float4(+1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 2) return(float4(+1.0f, -1.0f, 0.0f, 1.0f));
	if (nVertexID == 3) return(float4(-1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 4) return(float4(+1.0f, -1.0f, 0.0f, 1.0f));
	if (nVertexID == 5) return(float4(-1.0f, -1.0f, 0.0f, 1.0f));

	return(float4(0, 0, 0, 0));
}

PS_LRT_OUTPUT PSLightTargeet(float4 position : SV_POSITION)
{
	PS_LRT_OUTPUT output; 

	//float3 cColor = gtxtScene[int2(position.xy)].rgb;
	//float fDepth = gtxtDepth[int2(position.xy)].r;

	//float3 cFogColor = float3(0.15f, 0.15f, 0.15f);
	float3 vPosition = float3(position.x, position.y, gtxtDepth[position.xy].r);
	vPosition = mul(mul(vPosition, gmtxInverseProjection), gmtxInverseView);
	float3 vToCamera = gvCameraPosition.xyz - vPosition;
	float4 vNormal = gtxtNormal[int2(position.xy)];
	float4 vLightedColor = float4(gvCameraPosition.x,1,1, 1.0f);
		// BlinnPhong(gLights[0].m_cDiffuse, gLights[0].m_vDirection, vNormal, vToCamera);
	output.Light = vLightedColor;
	return output;
}