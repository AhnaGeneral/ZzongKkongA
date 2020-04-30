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

	float vPorjPosZ = gtxtDepth[int2(position.xy)].y * 300.f;
	float uvX = position.x / 760;
	uvX = (uvX * 2.0f) - 1.0f;
	float uvY = position.y / 600;
	uvY = (uvY * 2.0f) - 1.0f;

	float4 vPosition;
	vPosition.x = uvX * vPorjPosZ;
	vPosition.y = uvY * vPorjPosZ;
	vPosition.z = gtxtDepth[int2(position.xy)].x * vPorjPosZ;
	vPosition.w = 1 * vPorjPosZ;

	vPosition = mul(mul(vPosition, gmtxInverseProjection), gmtxInverseView);

	float4 vNormal = gtxtNormal[int2(position.xy)];
	output.Light = Lighting(vPosition.xyz, vNormal.xyz);

	float weight = 0;
	//±Û·Î¿ì¸Ê
	
	for (int i = 0; i < 4; i++)
	{
		weight = ((7 - i) * (7- i) / 3);
		for (int j = 0; j < 9; j++)
		{
			float4 cNonLight = gtxtNonLightNoise[int2(position.xy) + (gnOffsets[j] * i)];

			if (cNonLight.r == 22.0f/255.0f && cNonLight.g == 138.0f / 255.0f&
				cNonLight.b == 139.0f/255.0f)
				output.Light += cNonLight * weight;
		}
	}

	return output;

}