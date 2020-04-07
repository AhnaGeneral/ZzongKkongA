#include "stdafx.h"
#include "Object_Nosie.h"
#include "Shader_Noise.h"
#include "Mesh.h"

CObjectNosie::CObjectNosie(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CTriangleRect* pNoise = new CTriangleRect(pd3dDevice, pd3dCommandList, 50.0f, 50.0f, 1.0f, 1.0f);
	SetMesh(pNoise);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	NoiseSetTexture(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

CObjectNosie::~CObjectNosie()
{
	ReleaseShaderVariables();
}

void CObjectNosie::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// GameObject[게임 오브젝트 콘스탄트버퍼] -------------------------------------------------------------
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbGameObjects->Map(0, NULL, (void**)&m_pcbMappedGameObjects);

	// noisebuffer [노이즈의 콘스탄트버퍼] ----------------------------------------------------------------
	ncbElementBytes = ((sizeof(CB_NOISEBUFFERTYPE) + 255) & ~255);
	m_pd3dcbNoiseBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbNoiseBuffer->Map(0, NULL, (void**)&m_pcbMappedNoiseBuffers);

	// distort [외각 콘스탄트버퍼] ------------------------------------------------------------------------
	ncbElementBytes = ((sizeof(CB_DISTORTIONBUFFERTYPE) + 255) & ~255);
	m_pd3dcbDistortionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbDistortionBuffer->Map(0, NULL, (void**)&m_pcbMappdeDistortBuffers);
}

void CObjectNosie::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CObjectNosie::ReleaseShaderVariables()
{
	// GameObject[게임 오브젝트 콘스탄트버퍼] -------------------------------------------------------------
	//if (m_pd3dcbGameObjects)
	//{
	//	m_pd3dcbGameObjects->Unmap(0, NULL);
	//	m_pd3dcbGameObjects->Release();
	//}
	//if (m_pcbMappedGameObjects) delete m_pcbMappedGameObjects;

	// noisebuffer [노이즈의 콘스탄트버퍼] ----------------------------------------------------------------
	if (m_pd3dcbNoiseBuffer)
	{
		m_pd3dcbNoiseBuffer->Unmap(0, NULL);
		m_pd3dcbNoiseBuffer->Release();
	}
	//if (m_pcbMappedNoiseBuffers) delete m_pcbMappedNoiseBuffers;

	// distort [외각 콘스탄트버퍼] ------------------------------------------------------------------------
	if (m_pd3dcbDistortionBuffer)
	{
		m_pd3dcbDistortionBuffer->Unmap(0, NULL);
		m_pd3dcbDistortionBuffer->Release();
	}
	//if (m_pcbMappdeDistortBuffers) delete m_pcbMappdeDistortBuffers;
}


void CObjectNosie::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	static float frameTime = 0.0f;

	// 프레임 시간 카운터를 증가시킵니다.
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	// GameObject[게임 오브젝트 콘스탄트버퍼] -------------------------------------------------------------
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 16, &xmf4x4World, 0);

	UINT getobjectID = 0;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOT_PARAMETER_OBJECT, 4, &getobjectID, 16);

	// noisebuffer [노이즈의 콘스탄트버퍼] ----------------------------------------------------------------
	::memcpy(&m_pcbMappedNoiseBuffers->frameTime, &frameTime, sizeof(float));
	::memcpy(&m_pcbMappedNoiseBuffers->scrollSpeeds, &m_cbvNoisebuffer.scrollSpeeds, sizeof(XMFLOAT3));
	::memcpy(&m_pcbMappedNoiseBuffers->scales, &m_cbvNoisebuffer.scales, sizeof(XMFLOAT3));
	::memcpy(&m_pcbMappedNoiseBuffers->padding, &m_cbvNoisebuffer.padding, sizeof(float));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbNoiseBuffer->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_NOISEBUFFER, d3dGpuVirtualAddress);

	//std::cout << "frameTime "    << m_pcbMappedNoiseBuffers->frameTime  << std::endl;
	//std::cout << "scrollSpeeds " << m_pcbMappedNoiseBuffers->scrollSpeeds.x << ", " 
	//	                         << m_pcbMappedNoiseBuffers->scrollSpeeds.y <<", "
	//	                         << m_pcbMappedNoiseBuffers->scrollSpeeds.z << std::endl;


	// distort [외각 콘스탄트버퍼] ------------------------------------------------------------------------
	::memcpy(&m_pcbMappdeDistortBuffers->distortion1, &m_cbvdistortion.distortion1, sizeof(XMFLOAT2));
	::memcpy(&m_pcbMappdeDistortBuffers->distortion2, &m_cbvdistortion.distortion2, sizeof(XMFLOAT2));
	::memcpy(&m_pcbMappdeDistortBuffers->distortion3, &m_cbvdistortion.distortion3, sizeof(XMFLOAT2));
	::memcpy(&m_pcbMappdeDistortBuffers->distortionScale, &m_cbvdistortion.distortionScale, sizeof(float));
	::memcpy(&m_pcbMappdeDistortBuffers->distortionBias, &m_cbvdistortion.distortionBias, sizeof(float));

	d3dGpuVirtualAddress = m_pd3dcbDistortionBuffer->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_DISTORTIONBUFFER, d3dGpuVirtualAddress);

	//std::cout << "distortion1 " << float(m_pcbMappdeDistortBuffers->distortion1.x) << ", " << m_pcbMappdeDistortBuffers->distortion1.y << std::endl;
	//std::cout << "distortion2 " << m_pcbMappdeDistortBuffers->distortion2.x << ", " << m_pcbMappdeDistortBuffers->distortion2.y << std::endl;
	//std::cout << "distortion3 " << m_pcbMappdeDistortBuffers->distortion3.x << ", " << m_pcbMappdeDistortBuffers->distortion3.y << std::endl;
}

void CObjectNosie::GenerateShaderDistortionBuffer()
{
	m_cbvdistortion.distortion1 = XMFLOAT2(0.1f, 0.2f);
	m_cbvdistortion.distortion2 = XMFLOAT2(0.1f, 0.3f);
	m_cbvdistortion.distortion3 = XMFLOAT2(0.1f, 0.1f);
	m_cbvdistortion.distortionScale = 0.8f;
	m_cbvdistortion.distortionBias = 0.5f;

	m_cbvNoisebuffer.scrollSpeeds = XMFLOAT3(1.f, 1.1f, 1.3f);
	m_cbvNoisebuffer.padding = 0.0;
	m_cbvNoisebuffer.scales = XMFLOAT3(1.0f, 2.0f, 3.0f);

	// 처음부터 초기화 할시.
	//   m_pcbMappdeDistortBuffers->distortion1 = XMFLOAT2(0.1f, 0.2f);
	//   m_pcbMappdeDistortBuffers->distortion2 = XMFLOAT2(0.1f, 0.3f);
	//   m_pcbMappdeDistortBuffers->distortion3 = XMFLOAT2(0.1f, 0.1f);
	//   m_pcbMappdeDistortBuffers->distortionScale = 0.8f; 
	//   m_pcbMappdeDistortBuffers->distortionBias = 0.5f; 
}

void CObjectNosie::UpdateShaderNoiseBuffer(float Time, XMFLOAT3 scrollspeed, XMFLOAT3 scales, float padding)
{
	m_cbvNoisebuffer.frameTime = Time;
	m_cbvNoisebuffer.scrollSpeeds = scrollspeed;
	m_cbvNoisebuffer.scales = scales;
	m_cbvNoisebuffer.padding = padding;
}


void CObjectNosie::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	m_ppMaterials[0]->m_pShader->Render(pd3dCommandList, pCamera, 0);
	m_ppMaterials[0]->UpdateShaderVariable(pd3dCommandList);
	// 2020 03 23 텍스처 여러개 불렀을 때 해결 방법
	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList, 0);
}

void CObjectNosie::NoiseSetTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CTexture* pNoiseTexture = new CTexture(3, RESOURCE_TEXTURE2D, 0);
	pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Noise/fire01.dds", 0);
	pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Noise/alpha01.dds", 1);
	pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Noise/noise01.dds", 2);

	Shader_Noise* pNosieShader = new Shader_Noise();
	pNosieShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 5);
	pNosieShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pNosieShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 3); // 힙이 달라서... OnPrepareRender
	pNosieShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pNoiseTexture, ROOT_PARAMETER_NOISE_TEX, true);
	SetShader(pNosieShader);
	m_ppMaterials[0]->SetTexture(pNoiseTexture);

}


CObjectFog::CObjectFog(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
	: CObjectNosie(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature)
{
	CTriangleRect* pNoise = new CTriangleRect(pd3dDevice, pd3dCommandList, 50.0f, 50.0f, 1.0f, 1.0f);
	SetMesh(pNoise);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	NoiseSetTexture(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

CObjectFog::~CObjectFog()
{
}

void CObjectFog::NoiseSetTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CTexture* pNoiseTexture = new CTexture(3, RESOURCE_TEXTURE2D, 0);
	pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Fog/Blur.dds", 0);
	pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Fog/Fog.dds", 1);
	pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Fog/Fog2.dds", 2);

	Shader_FogNoise* pNosieShader = new Shader_FogNoise();
	pNosieShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 5);
	pNosieShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pNosieShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 3); // 힙이 달라서... OnPrepareRender
	pNosieShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pNoiseTexture, ROOT_PARAMETER_FOG_TEX, true);
	SetShader(pNosieShader);
	m_ppMaterials[0]->SetTexture(pNoiseTexture);
}

void CObjectFog::GenerateShaderDistortionBuffer()
{
	m_cbvdistortion.distortion1 = XMFLOAT2(0.1f, 0.2f);
	m_cbvdistortion.distortion2 = XMFLOAT2(0.1f, 0.3f);
	m_cbvdistortion.distortion3 = XMFLOAT2(0.1f, 0.1f);
	m_cbvdistortion.distortionScale = 0.8f;
	m_cbvdistortion.distortionBias = 0.5f;

	m_cbvNoisebuffer.scrollSpeeds = XMFLOAT3(0.02f, 0.01f, 2.3f);
	m_cbvNoisebuffer.padding = 0.0;
	m_cbvNoisebuffer.scales = XMFLOAT3(1.0f, 2.0f, 3.0f);
}


CObjectWater::CObjectWater(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
	: CObjectNosie(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature)
{
	CTriangleRect* pNoise = new CTriangleRect(pd3dDevice, pd3dCommandList, 5.0f, 5.0f, 1.0f, 1.0f);
	SetMesh(pNoise);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	NoiseSetTexture(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
}

CObjectWater::~CObjectWater()
{
}

void CObjectWater::NoiseSetTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CTexture* pNoiseTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pNoiseTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Geometry/Water/Water_NM.dds", 0);

	Shader_WaterNoise* pNosieShader = new Shader_WaterNoise();
	pNosieShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature, 5);
	pNosieShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pNosieShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 1); // 힙이 달라서... OnPrepareRender
	pNosieShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pNoiseTexture, ROOT_PARMAMETER_WATER_NORMAL_TEX, false);
	SetShader(pNosieShader);
	m_ppMaterials[0]->SetTexture(pNoiseTexture);
}

void CObjectWater::GenerateShaderDistortionBuffer()
{
	m_cbvdistortion.distortion1 = XMFLOAT2(0.1f, 0.2f);
	m_cbvdistortion.distortion2 = XMFLOAT2(0.1f, 0.3f);
	m_cbvdistortion.distortion3 = XMFLOAT2(0.1f, 0.1f);
	m_cbvdistortion.distortionScale = 0.8f;
	m_cbvdistortion.distortionBias = 0.5f;

	m_cbvNoisebuffer.scrollSpeeds = XMFLOAT3(0.3f, 2.1f, 0.02f);
	m_cbvNoisebuffer.padding = 0.0;
	m_cbvNoisebuffer.scales = XMFLOAT3(1.0f, 20.0f, 25.0f);
}
