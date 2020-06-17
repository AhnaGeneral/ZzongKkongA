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

	output.tex = input.tex;
	
	output.color = input.color; 

	return output; 
}


//  gtxtEffectTexture register 31 Texture 
//ParticlePixelShader
PS_NONLIGHT_MRT_OUTPUT ParticlePixelShader(PixelInputType input)
{
	PS_NONLIGHT_MRT_OUTPUT output; 

	float4 textureColor; 
	float4 finalColor; 

	textureColor = gtxtEffectTexture.Sample(gSamplerState, input.tex);
	finalColor = textureColor * input.color; 

	output.NonLight = finalColor; 
	return output;
}