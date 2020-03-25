#include "Shaders.hlsl"

Texture2D gtxtBaseColorNoiseTex : register(t19);
Texture2D gtxtAlphaNoiseTex : register(t20);
Texture2D gtxtNoiseTex : register(t21);

cbuffer DistortionBuffer : register(b23)
{
	float2 distortion1 : packoffset(c0);
	float2 distortion2 : packoffset(c0.z);
	float2 distortion3 : packoffset(c1);
	float distortionScale : packoffset(c1.z);
	float distortionBias : packoffset(c1.w);
};

cbuffer cbNoiseBuffer : register(b22)
{
	float    frameTime : packoffset(c0);
	float3   scrollSpeeds : packoffset(c0.y);
	float3   scales : packoffset (c1);
	float    padding : packoffset (c1.w);
};

struct VS_NOISE_INPUT
{
	float3 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PS_NOISE_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
};


PS_NOISE_INPUT NoiseVertexShader(VS_NOISE_INPUT input)
{
	PS_NOISE_INPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);

	output.tex = input.tex;

	output.tex1 = (input.tex * scales.x);
	output.tex1.y = output.tex1.y + (frameTime * scrollSpeeds.x);

	output.tex2 = (input.tex * scales.y);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	output.tex3 = (input.tex * scales.z);
	output.tex3.y = output.tex3.y + (frameTime * scrollSpeeds.z);

	return output;
}

float4 NoisePixelShader(PS_NOISE_INPUT input) : SV_TARGET
{

	//float2 noiseCoords;
	//float4 nosieColor, alphaColor;
	//리소스가 저장된 힙이 다르다 

	//1. 3개의 서로 다른 텍스처 좌표를 사용하여 동일한 노이즈 텍스처를 
	//   샘플링하여 3개의 다른 노이즈 스케일을 얻는다. 
	float4 noise1 = gtxtNoiseTex.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtNoiseTex.Sample(gSamplerState, input.tex2);
	float4 noise3 = gtxtNoiseTex.Sample(gSamplerState, input.tex3);

	//2.  (0, 1) 범위에서 (-1, +1)범위로 노이즈를 이동한다. 
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	//3. 개의 서로 다른 왜곡 x와 y값으로 세 개위 노이즈 x와 y좌표를 왜곡한다.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	//4. 세 가지 왜곡 된 노이즈 결과를 모두 하나의 노이즈 결과로 결합한다. 
	float4 finalNoise = noise1 + noise2 + noise3;

	//왜곡 스케일 및 바이어스 값에 의해 입력 텍스처 y 좌표를 왜곡한다.
	//위쪽 효과에서 불꽃이 깜박 거리는 텍스처를 위로 움직이면 섭동이 강해진다 ???? 뭔소리
	float perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	float2 noiseCoords;
	//화재 색상 텍스처를 샘플링하는 데 사용할 교란되고 왜곡 된 텍스처 샘플링 좌표를 만든다. 
	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;

	// 섭동되고 왜곡 된 텍스처 샘플링 좌표를 사용하여 화재 텍스처에서 색상을 샘플링합니다.
	// 화스랩 상태를 감싸는 대신 클램프 샘플 상태를 사용하여 화염을 감싸는 것을 방지합니다.
	float4 fireColor = gtxtBaseColorNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);

	// 교란되고 왜곡 된 텍스처 샘플링 좌표를 사용하여 알파 텍스처에서 알파 값을 샘플링합니다.
	// 이것은 불의 투명도에 사용됩니다.
	// 화스랩 상태를 감싸는 대신 클램프 샘플 상태를 사용하여 화염을 감싸는 것을 방지합니다.
	float4 alphaColor = gtxtAlphaNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);

	// 화재의 알파 블렌딩을 불안정하고 왜곡 된 알파 텍스처 값으로 설정합니다.
	fireColor.a = alphaColor;

	return fireColor;
}


PS_NOISE_INPUT FogVertexShader(VS_NOISE_INPUT input)
{
	PS_NOISE_INPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);

	output.tex = input.tex;

	output.tex1 = (input.tex * scales.x);
	output.tex1.y = output.tex1.y + (frameTime * scrollSpeeds.x);

	output.tex2 = (input.tex * scales.y);
	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);

	output.tex3 = (input.tex * scales.z);
	output.tex3.y = output.tex3.y + (frameTime * scrollSpeeds.z);

	return output;
}

float4 FogPixelShader(PS_NOISE_INPUT input) : SV_TARGET
{

	//float2 noiseCoords;
	//float4 nosieColor, alphaColor;
	//리소스가 저장된 힙이 다르다 

	//1. 3개의 서로 다른 텍스처 좌표를 사용하여 동일한 노이즈 텍스처를 
	//   샘플링하여 3개의 다른 노이즈 스케일을 얻는다. 
	float4 noise1 = gtxtNoiseTex.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtNoiseTex.Sample(gSamplerState, input.tex2);
	float4 noise3 = gtxtNoiseTex.Sample(gSamplerState, input.tex3);

	//2.  (0, 1) 범위에서 (-1, +1)범위로 노이즈를 이동한다. 
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	//3. 개의 서로 다른 왜곡 x와 y값으로 세 개위 노이즈 x와 y좌표를 왜곡한다.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	//4. 세 가지 왜곡 된 노이즈 결과를 모두 하나의 노이즈 결과로 결합한다. 
	float4 finalNoise = noise1 + noise2 + noise3;

	//왜곡 스케일 및 바이어스 값에 의해 입력 텍스처 y 좌표를 왜곡한다.
	//위쪽 효과에서 불꽃이 깜박 거리는 텍스처를 위로 움직이면 섭동이 강해진다 ???? 뭔소리
	float perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	float2 noiseCoords;
	//화재 색상 텍스처를 샘플링하는 데 사용할 교란되고 왜곡 된 텍스처 샘플링 좌표를 만든다. 
	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;

	// 섭동되고 왜곡 된 텍스처 샘플링 좌표를 사용하여 화재 텍스처에서 색상을 샘플링합니다.
	// 화스랩 상태를 감싸는 대신 클램프 샘플 상태를 사용하여 화염을 감싸는 것을 방지합니다.
	float4 fireColor = gtxtBaseColorNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);

	// 교란되고 왜곡 된 텍스처 샘플링 좌표를 사용하여 알파 텍스처에서 알파 값을 샘플링합니다.
	// 이것은 불의 투명도에 사용됩니다.
	// 화스랩 상태를 감싸는 대신 클램프 샘플 상태를 사용하여 화염을 감싸는 것을 방지합니다.
	float4 alphaColor = gtxtAlphaNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);

	// 화재의 알파 블렌딩을 불안정하고 왜곡 된 알파 텍스처 값으로 설정합니다.
	fireColor.a = alphaColor;

	return fireColor;
}