//#pragma once
//#include "Mesh.h"
//#include "Material.h"
//class System_Particle //ParticleSystemClass 
//{
//private:
//	struct ParticleType
//	{
//		float positionX, positionY, positionZ;
//		float red, green, blue;
//		float velocity;
//		bool active;
//	};
//	struct  VertexType
//	{
//		XMFLOAT3 position; 
//		XMFLOAT2 texture; 
//		XMFLOAT4 color; 
//	};
//
//private:
//	float m_particleDeviationX; //ÆÄÆ¼Å¬ ÀÏÅ» 
//	float m_particleDeviationY; //ÆÄÆ¼Å¬ ÀÏÅ» 
//	float m_particleDeviationZ; //ÆÄÆ¼Å¬ ÀÏÅ» 
//	float m_particleVelocityVariation = 0; 
//	float m_particleSize = 0; 
//	float m_particlesPerSecond = 0; 
//	int   m_maxParticles = 0; 
//
//	int   m_currentParticleCount = 0; 
//	float m_accumulatedTime = 0; 
//
//	int m_vertexCount = 0; 
//	int m_indexCount = 0; 
//
//	CTexture* m_Texture = NULL; 
//	ParticleType* m_particleList = NULL;
//	VertexType* m_vertices = NULL; 
//	// buffer init value ..
//	
//
//public:
//	System_Particle(); 
//	System_Particle(const System_Particle& constSystemParticle);
//	~System_Particle();
//
//	bool Initialize(ID3D12Device* pd3dDevice, void* Texture);
//	void Shutdown(); 
//	bool Frame(float framTime, ID3D12GraphicsCommandList* pd3dCommandList);
//	void Render(ID3D12GraphicsCommandList* pd3dCommandList); 
//
//	ID3D12Resource* GetTexture(); 
//	int GetIndexCount(); 
//	
//	void ReleaseTexture();
//
//	bool InitializeParticleSystem();
//	void ShutdownParticleSystem();
//
//	bool InitializeBuffers(ID3D12Device* pd3dDevice);
//	void ShutdownBuffers();
//
//	void EmitParticles(float);
//	void UpdateParticles(float);
//	void KillParticles();
//
//	bool UpdateBuffers(ID3D12GraphicsCommandList* pd3dCommandList);
//	void RenderBuffers(ID3D12GraphicsCommandList* pd3dCommandList);
//};
//
