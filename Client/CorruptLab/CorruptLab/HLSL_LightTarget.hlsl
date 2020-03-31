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
	float vPorjPosZ = gtxtDepth[int2(position.xy)].y * 500.f;
	float uvX = position.x / 720;
	float uvY = position.y / 600;

	float4 vPosition;
	vPosition.x = uvX * vPorjPosZ;
	vPosition.y = uvY * vPorjPosZ;
	vPosition.z = gtxtDepth[int2(position.xy)].x * vPorjPosZ;
	vPosition.w = 1 * vPorjPosZ;

	vPosition = mul(mul(vPosition, gmtxInverseProjection), gmtxInverseView);

	float3 vToCamera = normalize(gvCameraPosition.xyz - vPosition.xyz);
	float4 vNormal = gtxtNormal[int2(position.xy)];
	float4 vLightedColor = DirectionalLight(0, vNormal, vToCamera);
		
	output.Light = vLightedColor;
	return output;
}