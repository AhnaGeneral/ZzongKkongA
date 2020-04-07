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

	float vPorjPosZ = gtxtDepth[int2(position.xy)].y * 150.f;
	float uvX = position.x / 760;
	uvX = (uvX * 2.0f) - 1.0f;
	float uvY = position.y / 600;
	uvY = (uvY * 2.0f) - 1.0f;

	float4 vPosition;
	vPosition.x = uvX * vPorjPosZ;
	vPosition.y = uvY * vPorjPosZ;
	vPosition.z = gtxtDepth[int2(position.xy)].x * vPorjPosZ;
	vPosition.w = 1 * vPorjPosZ;


	//float4 vDepth = gtxtDepth[int2(position.xy)];
	//float valueW = vDepth.g * 150.0f; 
	//float valueZ = vDepth.r * valueW; 
	///*position.x = position.x * 2 - 1;
	//position.y = position.y * 2 - 1;*/

	//float4 vposition =  float4 (position.x /720 , position.y/600, valueZ, valueW);
	vPosition = mul(mul(vPosition, gmtxInverseProjection), gmtxInverseView);

	float4 vNormal = gtxtNormal[int2(position.xy)];
//	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);

	//float3 vToCamera = normalize(vCameraPosition - vPosition);
	output.Light = Lighting(vPosition.xyz, vNormal.xyz);
		//float4(position.x / 760, position.y / 600, 0, 1);
	return output;

}