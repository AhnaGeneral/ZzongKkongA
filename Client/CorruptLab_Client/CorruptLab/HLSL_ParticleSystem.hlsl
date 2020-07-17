#include "Shaders.hlsl"

struct vertexInputType
{
	float3 position : POSITION; 
	float2 tex : TEXCOORD0; 
	float4 color : COLOR;
};


struct GeometryInputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

struct GeometryOutputType
{
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

GeometryInputType particleVertexShader(vertexInputType input)
{
	GeometryInputType output;

	output.position = input.position;
	output.tex = input.tex;
	output.color = input.color;

	return output; 
}

[maxvertexcount(4)]
void ParticleGeometryShader(point GeometryInputType input[1],
	inout TriangleStream <GeometryOutputType> outStream)
{
	float3 vUP = float3(0.0f, 1.0f, 0.0f);
	float3 vLook = gvCameraPosition.xyz - input[0].position;
	vLook = normalize(vLook);
	float3 vRight = cross(vUP, vLook);

	float fHalfW = 10 * 0.5f;
	float fHalfH = 10 * 0.5f;

	float4 pVertices[4];
	pVertices[0] = float4(input[0].position + fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[1] = float4(input[0].position + fHalfW * vRight + fHalfH * vUP, 1.0f);
	pVertices[2] = float4(input[0].position - fHalfW * vRight - fHalfH * vUP, 1.0f);
	pVertices[3] = float4(input[0].position - fHalfW * vRight + fHalfH * vUP, 1.0f);

	float2 pUVs[4] = { float2 (0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

	GeometryOutputType output;

	for (int i = 0; i < 4; ++i)
	{
		output.posH = mul(mul(pVertices[i], gmtxView), gmtxProjection);
		output.tex = pUVs[i];
		output.color = input[0].color;
		outStream.Append(output);
	}
}

//  gtxtEffectTexture register 31 Texture 
//ParticlePixelShader
PS_NONLIGHT_MRT_OUTPUT ParticlePixelShader(GeometryOutputType input)
{
	PS_NONLIGHT_MRT_OUTPUT output; 

	float4 textureColor; 
	float4 finalColor; 

	textureColor = gtxtEffectTexture.Sample(gSamplerState, input.tex);
	finalColor = textureColor * input.color; 
	output.NonLight = finalColor; 
	return output;
}