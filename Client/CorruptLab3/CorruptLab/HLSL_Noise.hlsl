
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
//	//������ ��� ����� ���� ��ġ ���͸� 4������ �����Ѵ�. 
//	input.position.w = 1.0f; 
//
//	output.position = mul(mul(mul(input.position, gmtxGameObject), gmtxView), gmtxProjection); 
//
//	//�ȼ� ���̴��� �ؽ�ó ��ǥ�� �����Ѵ�. 
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
//	//3���� ���� �ٸ� �ؽ�ó ��ǥ�� ����Ͽ� ������ ������ �ؽ�ó�� ���ø��Ͽ� 3���� �ٸ� ������ �������� ����ϴ�.
//	noise1 = noiseTexture.Sample(SampleType, input.tex1);
//	noise2 = noiseTexture.Sample(SampleType, input.tex2);
//	noise3 = noiseTexture.Sample(SampleType, input.tex3);
//
//	//(0, 1) �������� (-1, +1)������ ����� �̵��Ѵ�. 
//	noise1 = (noise1 - 0.5f) * 2.0f;
//	noise2 = (noise2 - 0.5f) * 2.0f;
//	noise3 = (noise3 - 0.5f) * 2.0f;
//
//    //3 ���� ���� �ٸ� �ְ� x�� y������ �� ���� ������ x�� y��ǥ�� �ְ��Ѵ�.
//	noise1.xy = noise1.xy * distortion1.xy;
//	noise2.xy = noise2.xy * distortion2.xy;
//	noise3.xy = noise3.xy * distortion3.xy;
//
//	// �� ���� �ְ� �� ������ ����� ��� �ϳ��� ������ ����� �����Ѵ�. 
//	finalNosie = noise1 + noise2 + noise3;
//
//	//�ְ� ������ �� ���̾ ���� ���� �Է� �ؽ�ó y ��ǥ�� �ְ��Ѵ�.
//	//���� ȿ������ �Ҳ��� ���� �Ÿ��� �ؽ�ó�� ���� �����̸� ������ ��������
//	perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;
//
//	//ȭ�� ���� �ؽ�ó�� ���ø��ϴ� �� ����� �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� �����. 
//	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;
//
//	//���� �ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� ȭ�� �ؽ�ó���� ������ ���ø��Ѵ�
//	//ȭ���� ���¸� ���δ� ��� Ŭ���� ���� ���¸� ����Ͽ� ȭ���� ���δ� ���� �����Ѵ�. 
//	nosieColor = fireTexture.Sample(SampleType2, noiseCoords.xy);
//	alphaColor = alphaTexture.Sample(SampleType2, noiseCoords.xy);
//	// ȭ���� ���� ������ �Ҿ����ϰ� �ְ� �� ���� �ؽ�ó ������ �����մϴ�.
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

	//������ ��� ����� ���� ��ġ ���͸� 4������ �����Ѵ�.  
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.tex = input.tex;
	
	return output; 
}

float4 NoisePixelShader(PS_NOISE_INPUT input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}