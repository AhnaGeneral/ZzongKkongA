#include "Object_Effect.h"

CObject_Effect::CObject_Effect()
{
}

CObject_Effect::CObject_Effect(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, XMFLOAT3 position, CShader* pShader)
{
	m_pd3dPositionBuffer =
		::CreateBufferResource(pd3dDevice, pd3dCommandList, &position, sizeof(XMFLOAT3),
			D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_d3dPositionBufferView.StrideInBytes = sizeof(XMFLOAT3);
	m_d3dPositionBufferView.SizeInBytes = sizeof(XMFLOAT3);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

CObject_Effect::~CObject_Effect()
{
}

void CObject_Effect::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// GameObject[게임 오브젝트 콘스탄트버퍼] -------------------------------------------------------------
	UINT ncbElementBytes = ((sizeof(CB_EFFECT_ELEMENT) + 255) & ~255); //256의 배수
	m_pd3dEffectElementBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList,
		                       NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD,
		                       D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dEffectElementBuffer->Map(0, NULL, (void**)&m_pcbEffectElementBuffer);

}

void CObject_Effect::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CObject_Effect::ReleaseShaderVariables()
{
	if (m_pd3dPositionBuffer)
	{
		//	m_pd3dPositionBuffer->Unmap(0, NULL);
		m_pd3dPositionUploadBuffer->Release();
		m_pd3dPositionBuffer->Release();
	}

	if (m_pd3dEffectElementBuffer)
	{
		m_pd3dEffectElementBuffer->Unmap(0, NULL);
		m_pd3dEffectElementBuffer->Release();
	}
}

void CObject_Effect::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	::memcpy(&m_pcbEffectElementBuffer->TranslationPos, &m_TranslationPos, sizeof(XMFLOAT3));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dEffectElementBuffer->GetGPUVirtualAddress(); 
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_EFFECT_ELEMENT_BUFFER, d3dGpuVirtualAddress); 
}

void CObject_Effect::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[1] = { m_d3dPositionBufferView };
	pd3dCommandList->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);
	pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	pd3dCommandList->DrawInstanced(1, 1, 0, 0);
}

void CObject_Effect::TranslationUpdate(XMFLOAT3 pos)
{
	m_TranslationPos = pos; 
}
