#include "Shaders.hlsl"

struct vertexInputType
{
	float4 position : POSITION; 
	float2 tex : TEXCOORD0; 
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION; 
	float2 tex : TEXCOORD0; 
	float4 color :COLOR; 
};

PixelInputType particleVertexShader(vertexInputType input)
{
	PixelInputType output; 
	input.position.w = 1.0f; 

	output.position = mul(input.position, gmtxGameObject);
	output.position = mul(output.position, gmtxView); 
	output.position = mul(output.position, gmtxProjection);

	output.tex = intput.tex; 
	
	output.color = input.color; 

	return output; 
}
//
//샘플러 
//텍스처

Texture2D gtxtParticleTexture;

float4 ParticlePixelShader(PixelInputType input)
{
	float4 textureColor; 
	float4 finalColor; 

	textureColor = gtxtParticleTexture.Sample(gSamplerState, input.tex);
	finalColor = textureColor * input.color; 

	return finalColor; 
}