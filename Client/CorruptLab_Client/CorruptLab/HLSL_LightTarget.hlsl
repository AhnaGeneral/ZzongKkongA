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

	float vPorjPosZ = gtxtDepth[int2(position.xy)].y * 500.f;
	float uvX = position.x / 760;
	uvX = (uvX * 2.0f) - 1.0f;
	float uvY = position.y / 600;
	uvY = (uvY * 2.0f) - 1.0f;

	float4 vPosition;
	vPosition.x = uvX * vPorjPosZ;
	vPosition.y = -uvY * vPorjPosZ;
	vPosition.z = gtxtDepth[int2(position.xy)].x * vPorjPosZ;
	vPosition.w = 1 * vPorjPosZ;

	vPosition = mul(mul(vPosition, gmtxInverseProjection), gmtxInverseView);

	float4 vNormal = (gtxtNormal[int2(position.xy)] - 0.5f)*2;
	output.Light =  Lighting(vPosition.xyz, vNormal.xyz) * 1.5f;
				//float4(vPosition.x / 500, 1, vPosition.z / 500,1);

	return output;

}