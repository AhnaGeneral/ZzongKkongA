
#include "Shaders.hlsl"

//cbuffer NoiseBuffer
//{
//	float frameTime; 
//	float3 scrollSpeeds;
//	float3 scales; 
//	float padding; 
//};
//
struct VS_NOISE_INPUT
{
	float3 position : POSITION;
	float2 tex : TEXCOORD;
};

//struct PS_NOISE_INPUT
//{
//	float4 position : SV_POSITION;
//	float2 tex : TEXCOORD0;
//	float2 tex1 : TEXCOORD1;
//	float2 tex2 : TEXCOORD2;
//	float2 tex3 : TEXCOORD3;
//};

struct PS_NOISE_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

//PS_NOISE_INPUT NoiseVertexShader(VS_NOISE_INPUT input)
//{
//	PS_NOISE_INPUT output; 
//
//	//적절한 행렬 계산을 위해 위치 벡터를 4단위로 변경한다. 
//	input.position.w = 1.0f; 
//
//	output.position = mul(mul(mul(input.position, gmtxGameObject), gmtxView), gmtxProjection); 
//
//	//픽셀 쉐이더의 텍스처 좌표를 저장한다. 
//	output.tex = input.tex; 
//
//	output.tex1 = (input.tex * scales.x); 
//	output.tex1.y = output.tex1.y + (frameTime * scrollSpeeds.x);
//
//	output.tex2 = (input.tex * scales.y);
//	output.tex2.y = output.tex2.y + (frameTime * scrollSpeeds.y);
//
//	output.tex3 = (input.tex * scales.z);
//	output.tex3.y = output.tex3.y + (frameTime * scrollSpeeds.z);
//
//	return output;
//}
//
//float4 NoisePixelShader(PS_NOISE_INPUT input) : SV_TARGET
//{
//	float4 noise1 , noise2 , noise3 , finalNosie; 
//    float perturb;
//	float2 nosieCoords;
//	float4 nosieColor, alphaColor;
//
//	//3개의 서로 다른 텍스처 좌표를 사용하여 동일한 노이즈 텍스처를 샘플링하여 3개의 다른 노이즈 스케일을 얻습니다.
//	noise1 = noiseTexture.Sample(SampleType, input.tex1);
//	noise2 = noiseTexture.Sample(SampleType, input.tex2);
//	noise3 = noiseTexture.Sample(SampleType, input.tex3);
//
//	//(0, 1) 범위에서 (-1, +1)범위로 노이즈를 이동한다. 
//	noise1 = (noise1 - 0.5f) * 2.0f;
//	noise2 = (noise2 - 0.5f) * 2.0f;
//	noise3 = (noise3 - 0.5f) * 2.0f;
//
//    //3 개의 서로 다른 왜곡 x와 y값으로 세 개위 노이즈 x와 y좌표를 왜곡한다.
//	noise1.xy = noise1.xy * distortion1.xy;
//	noise2.xy = noise2.xy * distortion2.xy;
//	noise3.xy = noise3.xy * distortion3.xy;
//
//	// 세 가지 왜곡 된 노이즈 결과를 모두 하나의 노이즈 결과로 결합한다. 
//	finalNosie = noise1 + noise2 + noise3;
//
//	//왜곡 스케일 및 바이어스 값에 의해 입력 텍스처 y 좌표를 왜곡한다.
//	//위쪽 효과에서 불꽃이 깜박 거리는 텍스처를 위로 움직이면 섭동이 강해진다
//	perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;
//
//	//화재 색상 텍스처를 샘플링하는 데 사용할 교란되고 왜곡 된 텍스처 샘플링 좌표를 만든다. 
//	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;
//
//	//섭동 되고 왜곡 된 텍스처 샘플링 좌표를 사용하여 화재 텍스처에서 색상을 샘플링한다
//	//화스랩 상태를 감싸는 대신 클램프 샘플 상태를 사용하여 화염을 감싸는 것을 방지한다. 
//	nosieColor = fireTexture.Sample(SampleType2, noiseCoords.xy);
//	alphaColor = alphaTexture.Sample(SampleType2, noiseCoords.xy);
//	// 화재의 알파 블렌딩을 불안정하고 왜곡 된 알파 텍스처 값으로 설정합니다.
//	nosieColor.a = alphaColor;
//
//	return nosieColor; 
//}

Texture2D gtxtBaseColorNoiseTex : register(t19);
//Texture2D gtxtAlphaNoiseTex : register(t20);
//Texture2D gtxtNoiseTex : register(t21);

PS_NOISE_INPUT NoiseVertexShader(VS_NOISE_INPUT input)
{
	PS_NOISE_INPUT output; 

	//적절한 행렬 계산을 위해 위치 벡터를 4단위로 변경한다.  
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.tex = input.tex;
	
	return output; 
}

float4 NoisePixelShader(PS_NOISE_INPUT input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}