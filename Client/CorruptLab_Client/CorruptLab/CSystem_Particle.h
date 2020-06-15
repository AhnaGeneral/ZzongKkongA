#pragma once
#include "Object.h"

struct ParticleType
{
	float posX, posY, PosZ; 
	float red, green, blue; 
	float velocity; 
	bool active; 
};

struct VertexType
{
	XMFLOAT3 pos; 
	XMFLOAT2 tex; 
	XMFLOAT4 color; 
};

class ParticleSystemObject : public CGameObject
{
private:
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	float m_particleDeviationX = 0; //Deviation ¿œ≈ª, ∆Ì¬˜
	float m_particleDeviationY = 0;
	float m_particleDeviationZ = 0;
	float m_ParticleVelocity = 0; 
	float m_ParticleVelocityVariation = 0; //Variation ∫Ø»≠
	float m_ParticleSize = 0;
	float m_ParticlePerSecond = 0; 
	float m_maxParticles = 0; 

	int m_CurrentParticleCount = 0; 
	float m_accumulatedTime = 0; //accumulate √‡√¥

	CTexture* m_Texture = nullptr; 
	ParticleType* m_ParticleList = nullptr; 
	int m_vertexCount = 0; 
	int m_indexCount = 0;
	VertexType* m_Vertices = nullptr; 

	ID3D12Resource          * m_pd3dcbVertexBuffer = nullptr;
	ID3D12Resource          * m_pd3dVertexUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW  m_d3dVertexBufferView;
	VertexType*     m_pd3dVertices = nullptr;

	ID3D12Resource* m_indexBuffer = nullptr;

public:
	ParticleSystemObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~ParticleSystemObject();

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
     void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);

private:
	bool InitializeParticleSystem(); 
	void EmitParticles(float frameTime); 
	void UpdateParticles(float frameTime);
	void KillParticles();
	void ParticleSetTexture(ID3D12Device* pd3dDevice,
		ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);

};