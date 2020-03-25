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
	//���ҽ��� ����� ���� �ٸ��� 

	//1. 3���� ���� �ٸ� �ؽ�ó ��ǥ�� ����Ͽ� ������ ������ �ؽ�ó�� 
	//   ���ø��Ͽ� 3���� �ٸ� ������ �������� ��´�. 
	float4 noise1 = gtxtNoiseTex.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtNoiseTex.Sample(gSamplerState, input.tex2);
	float4 noise3 = gtxtNoiseTex.Sample(gSamplerState, input.tex3);

	//2.  (0, 1) �������� (-1, +1)������ ����� �̵��Ѵ�. 
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	//3. ���� ���� �ٸ� �ְ� x�� y������ �� ���� ������ x�� y��ǥ�� �ְ��Ѵ�.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	//4. �� ���� �ְ� �� ������ ����� ��� �ϳ��� ������ ����� �����Ѵ�. 
	float4 finalNoise = noise1 + noise2 + noise3;

	//�ְ� ������ �� ���̾ ���� ���� �Է� �ؽ�ó y ��ǥ�� �ְ��Ѵ�.
	//���� ȿ������ �Ҳ��� ���� �Ÿ��� �ؽ�ó�� ���� �����̸� ������ �������� ???? ���Ҹ�
	float perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	float2 noiseCoords;
	//ȭ�� ���� �ؽ�ó�� ���ø��ϴ� �� ����� �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� �����. 
	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;

	// �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� ȭ�� �ؽ�ó���� ������ ���ø��մϴ�.
	// ȭ���� ���¸� ���δ� ��� Ŭ���� ���� ���¸� ����Ͽ� ȭ���� ���δ� ���� �����մϴ�.
	float4 fireColor = gtxtBaseColorNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);

	// �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� ���� �ؽ�ó���� ���� ���� ���ø��մϴ�.
	// �̰��� ���� ������ ���˴ϴ�.
	// ȭ���� ���¸� ���δ� ��� Ŭ���� ���� ���¸� ����Ͽ� ȭ���� ���δ� ���� �����մϴ�.
	float4 alphaColor = gtxtAlphaNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);

	// ȭ���� ���� ������ �Ҿ����ϰ� �ְ� �� ���� �ؽ�ó ������ �����մϴ�.
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
	//���ҽ��� ����� ���� �ٸ��� 

	//1. 3���� ���� �ٸ� �ؽ�ó ��ǥ�� ����Ͽ� ������ ������ �ؽ�ó�� 
	//   ���ø��Ͽ� 3���� �ٸ� ������ �������� ��´�. 
	float4 noise1 = gtxtNoiseTex.Sample(gSamplerState, input.tex1);
	float4 noise2 = gtxtNoiseTex.Sample(gSamplerState, input.tex2);
	float4 noise3 = gtxtNoiseTex.Sample(gSamplerState, input.tex3);

	//2.  (0, 1) �������� (-1, +1)������ ����� �̵��Ѵ�. 
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	//3. ���� ���� �ٸ� �ְ� x�� y������ �� ���� ������ x�� y��ǥ�� �ְ��Ѵ�.
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	//4. �� ���� �ְ� �� ������ ����� ��� �ϳ��� ������ ����� �����Ѵ�. 
	float4 finalNoise = noise1 + noise2 + noise3;

	//�ְ� ������ �� ���̾ ���� ���� �Է� �ؽ�ó y ��ǥ�� �ְ��Ѵ�.
	//���� ȿ������ �Ҳ��� ���� �Ÿ��� �ؽ�ó�� ���� �����̸� ������ �������� ???? ���Ҹ�
	float perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	float2 noiseCoords;
	//ȭ�� ���� �ؽ�ó�� ���ø��ϴ� �� ����� �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� �����. 
	noiseCoords.xy = (finalNoise.xy * perturb) + input.tex.xy;

	// �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� ȭ�� �ؽ�ó���� ������ ���ø��մϴ�.
	// ȭ���� ���¸� ���δ� ��� Ŭ���� ���� ���¸� ����Ͽ� ȭ���� ���δ� ���� �����մϴ�.
	float4 fireColor = gtxtBaseColorNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);

	// �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� ���� �ؽ�ó���� ���� ���� ���ø��մϴ�.
	// �̰��� ���� ������ ���˴ϴ�.
	// ȭ���� ���¸� ���δ� ��� Ŭ���� ���� ���¸� ����Ͽ� ȭ���� ���δ� ���� �����մϴ�.
	float4 alphaColor = gtxtAlphaNoiseTex.Sample(gSamplerClamp, noiseCoords.xy);

	// ȭ���� ���� ������ �Ҿ����ϰ� �ְ� �� ���� �ؽ�ó ������ �����մϴ�.
	fireColor.a = alphaColor;

	return fireColor;
}