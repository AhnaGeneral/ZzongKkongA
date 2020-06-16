//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "Scene.h"
#include "Object_Terrain.h"
#include "Mgr_Collision.h"
#include "Object_ItemBox.h"
#include "Object_DynamicObj.h"
#include "Monster_Yangmal.h"
#include "Mgr_Item.h"
#include "Mgr_Radiation.h"
#include "Object_DrugMaker.h"
#include "Shader_Effect.h"


CGameScene::CGameScene()
{
	m_pPlayer = NULL;
	m_pTerrain = NULL;
	m_pSkyBox = NULL;
	m_pCObjectWater = NULL;

	WindowCursorPos = { 0L,0L };

	n_ReactItem = 3;
	CItemMgr::GetInstance()->SetReactItem(ITEM_NONE);

	itemRange = 0.0f;

	m_bPipelineStateIndex = 0;
	m_ptOldCursorPos = {0L, 0L};

	m_nDynamicObjectTypeNum = 0;
	m_nStaticObjectTypeNum = 0; 
	m_nMonsterTypeNum = 0;

	m_pStaticObjLists = NULL;
	m_pDynamicObjLists = NULL;
	m_pMonsterLists = NULL;
	m_pSoftParticleShader = NULL;
	m_pSpecialFogShader = NULL;

	m_pShadowCamera = NULL;

	m_pShadowMap = NULL;
	m_pDepthTex = NULL;
}

CGameScene::~CGameScene()
{
	CItemMgr::GetInstance()->Destroy();
	CCollisionMgr::GetInstance()->Destroy();
}

void CGameScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	XMFLOAT3 xmf3Scale(2.0f, 0.6f, 2.0f);
	XMFLOAT4 xmf4Color(0.6f, 0.5f, 0.2f, 0.0f);

	m_pShadowCamera = new CSunCamera(XMFLOAT3(550.f, 150.f, 130.f));

	m_pSkyBox = new CSkyBox(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature,
		_T("Geometry/OneStageTerrain.raw"), 257, 257, 17, 17, xmf3Scale, xmf4Color, m_pShadowMap);
	

	CRadationMgr::GetInstance()->Initialize();
	m_pSoftParticleShader = new CSoftParticleShader();
	m_pSoftParticleShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, m_pDepthTex, m_pTerrain);

	m_pSpecialFogShader = new CShader_SpecialFog();
	m_pSpecialFogShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, m_pDepthTex, m_pTerrain);


	m_pCObjectWater = new CObjectWater(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature); //object

	m_pCObjectWater->SetPosition(XMFLOAT3(256.0f, 25.0f, 256.0f));
	m_pCObjectWater->Rotate(90.0f, 0.0f, 0.0f);
	m_pCObjectWater->GenerateShaderDistortionBuffer();

	m_pTestEffect = new CShader_Effect(); 
	m_pTestEffect->BuildObjects(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, NULL, m_pTerrain);

	CItemMgr::GetInstance()->Initialize(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	CCollisionMgr::GetInstance()->Initialize();
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CGameScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature)
		m_pd3dGraphicsRootSignature->Release();

	ReleaseShaderVariables();

	if (m_pSkyBox)
	{
		m_pSkyBox->Release();
		m_pSkyBox = NULL;
	}

	if (m_pTerrain)
	{
		m_pTerrain->Release();
		m_pTerrain = NULL;
	}

	if (m_pCObjectWater) 
	{
		m_pCObjectWater->ReleaseUploadBuffers();
		m_pCObjectWater->Release();
		m_pCObjectWater = NULL;
	}

	//----------------------------------------------
	if (m_pStaticObjLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		{
			for (auto Obj : *m_pStaticObjLists[i])
			{
				Obj->Release();
			}
			m_pStaticObjLists[i]->clear();
		}
	}
	delete[] m_pStaticObjLists; 
	m_pStaticObjLists = NULL;

	//----------------------------------------------
	if (m_pDynamicObjLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		{
			for (auto Obj : *m_pDynamicObjLists[i])
			{
				Obj->Release();
			}
			m_pDynamicObjLists[i]->clear();
		}
	}
	delete[] m_pDynamicObjLists; 
	m_pDynamicObjLists = NULL;

	//----------------------------------------------
	if (m_pMonsterLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nMonsterTypeNum; i++)
		{
			for (auto Obj : *m_pMonsterLists[i])
			{
				Obj->Release();
			}
			m_pMonsterLists[i]->clear();
		}
	}
	delete[] m_pMonsterLists;
	m_pMonsterLists = NULL;

	//----------------------------------------------
	if (m_pSoftParticleShader)
	{
		m_pSoftParticleShader->Release();
		m_pSoftParticleShader = NULL;
	}

	if (m_pSpecialFogShader)
	{
		m_pSpecialFogShader->Release();
		m_pSpecialFogShader = NULL;
	}
	//----------------------------------------------
	if (m_pShadowCamera)
	{
		m_pShadowCamera->ReleaseShaderVariables();
		delete m_pShadowCamera;
	}
	if (m_pShadowMap)
	{
		m_pShadowMap->ReleaseUploadBuffers();
		m_pShadowMap->Release();
		m_pShadowMap = NULL;
	}
	//if (m_pDepthTex)
	//{
	//	m_pDepthTex->ReleaseUploadBuffers();
	//	//m_pDepthTex->Release(); 다른 곳에서 하나 ..? 
	//	m_pDepthTex = NULL; 
	//}
	//----------------------------------------------

}
void CGameScene::ReleaseUploadBuffers()
{
	//----------------------------------------------
	if (m_pSkyBox) m_pSkyBox->ReleaseUploadBuffers();
	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();
	if (m_pPlayer) m_pPlayer->ReleaseUploadBuffers();
	//----------------------------------------------
	if (m_pStaticObjLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		{
			if (m_pStaticObjLists[i])
			{
				for(auto & obj: *m_pStaticObjLists[i])
					obj->ReleaseUploadBuffers();
			}
		}
	}
	//----------------------------------------------
	if (m_pDynamicObjLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		{
			for (auto Obj : *m_pDynamicObjLists[i])
			{
				Obj->ReleaseUploadBuffers();
			}
		}
	}
	//----------------------------------------------
	if (m_pMonsterLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nMonsterTypeNum; i++)
		{
			for (auto Obj : *m_pMonsterLists[i])
			{
				Obj->ReleaseUploadBuffers();
			}
		}
	}
	//----------------------------------------------
}


void CGameScene::CheckCollisions()
{
	CheckPlayerCollision();
}

void CGameScene::CheckPlayerCollision()
{
}

ID3D12RootSignature* CGameScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;

	D3D12_DESCRIPTOR_RANGE pd3dMaterialTexRanges[5];

	pd3dMaterialTexRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[0].NumDescriptors = 1;
	pd3dMaterialTexRanges[0].BaseShaderRegister = 7; //t4: gtxtAlbedoTexture
	pd3dMaterialTexRanges[0].RegisterSpace = 0;
	pd3dMaterialTexRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[1].NumDescriptors = 1;
	pd3dMaterialTexRanges[1].BaseShaderRegister = 8; //t7: gtxtSpecularTexture
	pd3dMaterialTexRanges[1].RegisterSpace = 0;
	pd3dMaterialTexRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[2].NumDescriptors = 1;
	pd3dMaterialTexRanges[2].BaseShaderRegister = 9; //t8: gtxtNormalTexture
	pd3dMaterialTexRanges[2].RegisterSpace = 0;
	pd3dMaterialTexRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[3].NumDescriptors = 1;
	pd3dMaterialTexRanges[3].BaseShaderRegister = 10; //t9: gtxtMetallicTexture
	pd3dMaterialTexRanges[3].RegisterSpace = 0;
	pd3dMaterialTexRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[4].NumDescriptors = 1;
	pd3dMaterialTexRanges[4].BaseShaderRegister = 11; //t10: gtxtEmissionTexture
	pd3dMaterialTexRanges[4].RegisterSpace = 0;
	pd3dMaterialTexRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dSkyTexRange;
	pd3dSkyTexRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dSkyTexRange.NumDescriptors = 1;
	pd3dSkyTexRange.BaseShaderRegister = 12; // t13:gtxtSkyCubeTexture
	pd3dSkyTexRange.RegisterSpace = 0;
	pd3dSkyTexRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dCloudTexRange;
	pd3dCloudTexRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dCloudTexRange.NumDescriptors = 1;
	pd3dCloudTexRange.BaseShaderRegister = 13; // t14:gtxtCloudTexture
	pd3dCloudTexRange.RegisterSpace = 0;
	pd3dCloudTexRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dNoiseTexRanges;  // ( 12~14 ) firenosie 
	pd3dNoiseTexRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dNoiseTexRanges.NumDescriptors = 3; 
	pd3dNoiseTexRanges.BaseShaderRegister = 14;
	pd3dNoiseTexRanges.RegisterSpace = 0;
	pd3dNoiseTexRanges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dFogTexRanges;    //( 15 ~ 17 ) Fognoise
	pd3dFogTexRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dFogTexRanges.NumDescriptors = 3;
	pd3dFogTexRanges.BaseShaderRegister = 17;
	pd3dFogTexRanges.RegisterSpace = 0;
	pd3dFogTexRanges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dWaterTexRanges;  // ( 18 ) waternoise
	pd3dWaterTexRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dWaterTexRanges.NumDescriptors = 1; 
	pd3dWaterTexRanges.BaseShaderRegister = 20;
	pd3dWaterTexRanges.RegisterSpace = 0;
	pd3dWaterTexRanges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dTerrainTexRanges; // ( 50 ~ 62 ) onsstageterrain
	pd3dTerrainTexRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dTerrainTexRanges.NumDescriptors = 12;
	pd3dTerrainTexRanges.BaseShaderRegister = 50;
	pd3dTerrainTexRanges.RegisterSpace = 0;
	pd3dTerrainTexRanges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dShadowMapTex; // ( 50 ~ 62 ) onsstageterrain
	pd3dShadowMapTex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dShadowMapTex.NumDescriptors = 1;
	pd3dShadowMapTex.BaseShaderRegister = 22;
	pd3dShadowMapTex.RegisterSpace = 0;
	pd3dShadowMapTex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dSceneDepthTex; // ( 50 ~ 62 ) onsstageterrain
	pd3dSceneDepthTex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dSceneDepthTex.NumDescriptors = 1;
	pd3dSceneDepthTex.BaseShaderRegister = 23;
	pd3dSceneDepthTex.RegisterSpace = 0;
	pd3dSceneDepthTex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dEffectTex; 
	pd3dEffectTex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dEffectTex.NumDescriptors = 1;
	pd3dEffectTex.BaseShaderRegister = 70;
	pd3dEffectTex.RegisterSpace = 0;
	pd3dEffectTex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dDissolveTexRanges;  // ( 69 ) firenosie 
	pd3dDissolveTexRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDissolveTexRanges.NumDescriptors = 1;
	pd3dDissolveTexRanges.BaseShaderRegister = 69;
	pd3dDissolveTexRanges.RegisterSpace = 0;
	pd3dDissolveTexRanges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER pd3dRootParameters[23];

	pd3dRootParameters[ROOT_PARAMETER_CAMERA].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_CAMERA].Descriptor.ShaderRegister = 1; //b1 : Camera
	pd3dRootParameters[ROOT_PARAMETER_CAMERA].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_CAMERA].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_OBJECT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[ROOT_PARAMETER_OBJECT].Constants.Num32BitValues = 32;
	pd3dRootParameters[ROOT_PARAMETER_OBJECT].Constants.ShaderRegister = 2; //b2 : GameObject
	pd3dRootParameters[ROOT_PARAMETER_OBJECT].Constants.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_OBJECT].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_ALBEDO_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_ALBEDO_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_ALBEDO_TEX].DescriptorTable.pDescriptorRanges = &pd3dMaterialTexRanges[0];
	pd3dRootParameters[ROOT_PARAMETER_ALBEDO_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_SPECULAR_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_SPECULAR_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_SPECULAR_TEX].DescriptorTable.pDescriptorRanges = &pd3dMaterialTexRanges[1];
	pd3dRootParameters[ROOT_PARAMETER_SPECULAR_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_NORMAL_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_NORMAL_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_NORMAL_TEX].DescriptorTable.pDescriptorRanges = &pd3dMaterialTexRanges[2];
	pd3dRootParameters[ROOT_PARAMETER_NORMAL_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_METALLIC_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_METALLIC_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_METALLIC_TEX].DescriptorTable.pDescriptorRanges = &pd3dMaterialTexRanges[3];
	pd3dRootParameters[ROOT_PARAMETER_METALLIC_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_EMISSION_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_EMISSION_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_EMISSION_TEX].DescriptorTable.pDescriptorRanges = &pd3dMaterialTexRanges[4];
	pd3dRootParameters[ROOT_PARAMETER_EMISSION_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_BONE_TRANSFORMS].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_BONE_TRANSFORMS].Descriptor.ShaderRegister = 8; //Skinned Bone Transforms
	pd3dRootParameters[ROOT_PARAMETER_BONE_TRANSFORMS].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_BONE_TRANSFORMS].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[ROOT_PARAMETER_BONE_OFFSETS].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_BONE_OFFSETS].Descriptor.ShaderRegister = 7; //Skinned Bone Offsets
	pd3dRootParameters[ROOT_PARAMETER_BONE_OFFSETS].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_BONE_OFFSETS].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[ROOT_PARAMETER_SKYBOX_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_SKYBOX_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_SKYBOX_TEX].DescriptorTable.pDescriptorRanges = &pd3dSkyTexRange;
	pd3dRootParameters[ROOT_PARAMETER_SKYBOX_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_CLOUD_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_CLOUD_TEX].DescriptorTable.NumDescriptorRanges = 1;  // billboard
	pd3dRootParameters[ROOT_PARAMETER_CLOUD_TEX].DescriptorTable.pDescriptorRanges = &pd3dCloudTexRange;
	pd3dRootParameters[ROOT_PARAMETER_CLOUD_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_TERRAIN_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_TERRAIN_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_TERRAIN_TEX].DescriptorTable.pDescriptorRanges = &pd3dTerrainTexRanges;
	pd3dRootParameters[ROOT_PARAMETER_TERRAIN_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARMAMETER_WATER_NORMAL_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARMAMETER_WATER_NORMAL_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARMAMETER_WATER_NORMAL_TEX].DescriptorTable.pDescriptorRanges = &pd3dWaterTexRanges;
	pd3dRootParameters[ROOT_PARMAMETER_WATER_NORMAL_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_NOISEBUFFER].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_NOISEBUFFER].Constants.ShaderRegister = 9;
	pd3dRootParameters[ROOT_PARAMETER_NOISEBUFFER].Constants.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_NOISEBUFFER].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_DISTORTIONBUFFER].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_DISTORTIONBUFFER].Constants.ShaderRegister = 10;
	pd3dRootParameters[ROOT_PARAMETER_DISTORTIONBUFFER].Constants.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_DISTORTIONBUFFER].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_NOISE_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_NOISE_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_NOISE_TEX].DescriptorTable.pDescriptorRanges = &pd3dNoiseTexRanges;
	pd3dRootParameters[ROOT_PARAMETER_NOISE_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_FOG_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_FOG_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_FOG_TEX].DescriptorTable.pDescriptorRanges = &pd3dFogTexRanges;
	pd3dRootParameters[ROOT_PARAMETER_FOG_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_SHADOWCAMERA].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_SHADOWCAMERA].Descriptor.ShaderRegister = 5; //b1 : Camera
	pd3dRootParameters[ROOT_PARAMETER_SHADOWCAMERA].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_SHADOWCAMERA].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_SHADOWMAP].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_SHADOWMAP].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_SHADOWMAP].DescriptorTable.pDescriptorRanges = &pd3dShadowMapTex;
	pd3dRootParameters[ROOT_PARAMETER_SHADOWMAP].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_SCENEDEPTHTEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_SCENEDEPTHTEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_SCENEDEPTHTEX].DescriptorTable.pDescriptorRanges = &pd3dSceneDepthTex;
	pd3dRootParameters[ROOT_PARAMETER_SCENEDEPTHTEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_RADIATIONLEVEL].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_RADIATIONLEVEL].Constants.ShaderRegister = 11;
	pd3dRootParameters[ROOT_PARAMETER_RADIATIONLEVEL].Constants.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_RADIATIONLEVEL].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_EFFECT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_EFFECT].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_EFFECT].DescriptorTable.pDescriptorRanges = &pd3dEffectTex;
	pd3dRootParameters[ROOT_PARAMETER_EFFECT].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_DISSOLVE].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_DISSOLVE].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_DISSOLVE].DescriptorTable.pDescriptorRanges = &pd3dDissolveTexRanges;
	pd3dRootParameters[ROOT_PARAMETER_DISSOLVE].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc[2];
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc[0].MipLODBias = 0;
	d3dSamplerDesc[0].MaxAnisotropy = 1;
	d3dSamplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc[0].MinLOD = 0;
	d3dSamplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[0].ShaderRegister = 0;
	d3dSamplerDesc[0].RegisterSpace = 0;
	d3dSamplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	d3dSamplerDesc[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;  	// skybox sempler 
	d3dSamplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDesc[1].MipLODBias = 0;
	d3dSamplerDesc[1].MaxAnisotropy = 1;
	d3dSamplerDesc[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc[1].MinLOD = 0;
	d3dSamplerDesc[1].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc[1].ShaderRegister = 1;
	d3dSamplerDesc[1].RegisterSpace = 0;
	d3dSamplerDesc[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = _countof(d3dSamplerDesc);
	d3dRootSignatureDesc.pStaticSamplers = d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);

	char* pErrorString = NULL;
	if (pd3dErrorBlob)
		pErrorString = (char*)pd3dErrorBlob->GetBufferPointer();
	
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);

}

void CGameScene::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pShadowCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CGameScene::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pPlayer) m_pShadowCamera->Update(m_pPlayer->GetCamera());
	m_pShadowCamera->UpdateShaderVariables(pd3dCommandList, ROOT_PARAMETER_SHADOWCAMERA);
}

void CGameScene::ReleaseShaderVariables()
{
}

bool CGameScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		WindowCursorPos = m_ptOldCursorPos;

		//ScreenToClient함수 전체화면 기준인 커서의를 해당 클라이언트 윈도우 기준으로 좌표를 변환해주는 함수 이다. 
		ScreenToClient(hWnd, &WindowCursorPos);
		//n_ReactItem
		itemRange = FRAME_BUFFER_HEIGHT / 10.0f; //60

		if (WindowCursorPos.y <= FRAME_BUFFER_HEIGHT && WindowCursorPos.y >= (FRAME_BUFFER_HEIGHT - itemRange))
		{
			if ((WindowCursorPos.x >= 0.0f) && (WindowCursorPos.x <= itemRange))
			{
				n_ReactItem = ITEM_TYPE_HANDLIGHT;
				CItemMgr::GetInstance()->SetReactItem(ITEM_TYPE_HANDLIGHT);
				CItemMgr::GetInstance()->UseItemToPlayer(ITEM_TYPE_HANDLIGHT);
			}
			if ((WindowCursorPos.x >= itemRange) && (WindowCursorPos.x <= (itemRange * 2)))
			{
				n_ReactItem = ITEM_TYPE_HPKIT;
				CItemMgr::GetInstance()->SetReactItem(ITEM_TYPE_HPKIT);
				CItemMgr::GetInstance()->UseItemToPlayer(ITEM_TYPE_HPKIT);
			}

			if ((WindowCursorPos.x >= itemRange * 2) && (WindowCursorPos.x <= (itemRange * 3)))
			{
				n_ReactItem = ITEM_TYPE_PILL;
				CItemMgr::GetInstance()->SetReactItem(ITEM_TYPE_PILL);
				CItemMgr::GetInstance()->UseItemToPlayer(ITEM_TYPE_PILL);
			}
		}
		else
		{
			n_ReactItem = ITEM_NONE;
			CItemMgr::GetInstance()->SetReactItem(ITEM_NONE);
		}

		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}

	return true;
}

bool CGameScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	float* m_fShadowPosition = m_pShadowCamera->GetShadowCameraPosition();

	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
			ChangeTerrainPipeline();
			break;
		case VK_F3:
			m_pTerrain->SetHeightFactor();
			break;
		case 'Q':
		case 'q':
			PurifyMonster();
			ItemBoxCheck();
			break;
		case 'A':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0] += 10.0f, m_fShadowPosition[1], m_fShadowPosition[2]);
			break;
		case 'S':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0] -= 10.0f, m_fShadowPosition[1], m_fShadowPosition[2]);
			break;
		case 'D':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0], m_fShadowPosition[1] += 10.0f, m_fShadowPosition[2]);
			break;
		case 'F':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0], m_fShadowPosition[1] -= 10.0f, m_fShadowPosition[2]);
			break;
		case 'G':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0], m_fShadowPosition[1], m_fShadowPosition[2]+=10.0f);
			break;
		case 'H':
			m_pShadowCamera->SetShadowCameraPosition(m_fShadowPosition[0], m_fShadowPosition[1], m_fShadowPosition[2]-=10.0f);
			break;
		case VK_SPACE:
			m_pPlayer->SetAttackState();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return true;
}

bool CGameScene::ProcessInput(UCHAR* pKeysBuffer, HWND hWnd)
{
	DWORD dwDirection = 0;
	if (pKeysBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
	if (pKeysBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
	if (pKeysBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
	if (pKeysBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
	if (pKeysBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
	if (pKeysBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;

	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;

	if (GetCapture() == hWnd)
	{
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}

	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			if (pKeysBuffer[VK_RBUTTON] & 0xF0)
			{
				m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
				m_pShadowCamera->Rotate(cyDelta, 0.0f, -cxDelta);

			}
			else
			{
				m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
			}
		}
		if (dwDirection)
			m_pPlayer->Move(dwDirection, m_fElapsedTime, true);
	}
	m_pPlayer->Update(m_fElapsedTime);

	return true;
}


void CGameScene::PurifyMonster()
{
	for (auto pObj : *m_pDynamicObjLists[OBJECT_TYPE_DRUGMAKER])
	{
		XMFLOAT3 ObjPos = pObj->GetPosition();
		XMFLOAT3 PlayerPos = m_pPlayer->GetPosition();

		float Distance = Vector3::Length(Vector3::Subtract(ObjPos, PlayerPos));
		if (Distance < 15)
		{
			CDrugMaker* pMaker = dynamic_cast<CDrugMaker*>(pObj);
			if (pMaker->m_bEnable)
				for (auto pMon : *m_pMonsterLists[pMaker->m_iMonsterType])
				{
					pMaker->m_bEnable = false;
					pMon->GetPurified();
				}
		}
	}
}
void CGameScene::AnimateObjects(float fTimeElapsed)
{

	//if (m_pMonsterLists) // 몬스터 Render
	//{
	//	for (int i = 0; i < m_nMonsterTypeNum; i++)
	//	{
	//		for (auto Obj : *m_pMonsterLists[i])
	//		{
	//		}
	//	}
	//}

}

void CGameScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	pd3dCommandList->OMSetStencilRef(1);
	if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera, m_bPipelineStateIndex);
	if (m_pCObjectWater) m_pCObjectWater->Render(pd3dCommandList, pCamera);

	if (m_pStaticObjLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		{
            for(auto & obj: *m_pStaticObjLists[i])
			{
				obj->UpdateTransform(NULL);
				obj->Render(pd3dCommandList, pCamera, 0);
			}
		}
	}

	if (m_pDynamicObjLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		{
			for (auto Obj : *m_pDynamicObjLists[i])
			{
				Obj->Update(m_fElapsedTime);
				Obj->UpdateTransform(NULL);
				Obj->Render(pd3dCommandList, pCamera, 0);
			}
		}
	}

	if (m_pMonsterLists) // 몬스터 Render
	{
		for (int i = 0; i < m_nMonsterTypeNum; i++)
		{
			for (auto Obj : *m_pMonsterLists[i])
			{
				XMFLOAT3 monsterpos = Obj->GetPosition();
				XMFLOAT3 playerpos = m_pPlayer->GetPosition();
				float Distance = Vector3::Length(Vector3::Subtract(monsterpos, playerpos));
				Obj->SetDistanceToPlayer(Distance);
				Obj->SetPlayerPosition(playerpos);

				Obj->Update(m_fElapsedTime, NULL, m_pTerrain);
				Obj->Animate(m_fElapsedTime, NULL, Obj->m_iTrackNumber);
				Obj->UpdateTransform(NULL);
				Obj->Render(pd3dCommandList, pCamera, 2);
			}
		}
	}
	CheckCollisions();


	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera);
	if (m_pSkyBox)  m_pSkyBox->Render(pd3dCommandList, pCamera);

	CItemMgr::GetInstance()->BillboardUIRender(pd3dCommandList, pCamera);

	//if (m_pTestEffect)m_pTestEffect->Render(pd3dCommandList, pCamera);
	if (m_pSoftParticleShader) m_pSoftParticleShader->Render(pd3dCommandList, pCamera);
	if (m_pSpecialFogShader) m_pSpecialFogShader->Render(pd3dCommandList, pCamera);
}


void CGameScene::DepthRender(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	m_pShadowCamera->SetViewportsAndScissorRects(pd3dCommandList);

	if (m_pPlayer) m_pShadowCamera->Update(m_pPlayer->GetCamera());

	if(m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera, 2);

	if (m_pStaticObjLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		{
			for(auto& obj : *m_pStaticObjLists[i])
			{
				obj->UpdateTransform(NULL);
				obj->Render(pd3dCommandList, pCamera, 1);
			}
		}
	}
	if (m_pDynamicObjLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		{
			for (auto Obj : *m_pDynamicObjLists[i])
			{
				Obj->UpdateTransform(NULL);
				Obj->Render(pd3dCommandList, pCamera, 1);
			}
		}
	}
	if (m_pMonsterLists) // 몬스터 Render
	{
		for (int i = 0; i < m_nMonsterTypeNum; i++)
		{
			for (auto Obj : *m_pMonsterLists[i])
			{
				
				Obj->Render(pd3dCommandList, pCamera, 1);
			}
		}
	}
	m_pPlayer->Render(pd3dCommandList, pCamera, 1);
}
void CGameScene::Update(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
	AnimateObjects(fTimeElapsed);

	m_pPlayer->Animate(fTimeElapsed, NULL);
	CItemMgr::GetInstance()->Update(fTimeElapsed);
	CRadationMgr::GetInstance()->Update(m_fElapsedTime);
}

void CGameScene::ItemBoxCheck()
{
	for (auto pObj : *m_pDynamicObjLists[OBJECT_TYPE_ITEMBOX])
	{
		XMFLOAT3 ObjPos = pObj->GetPosition();
		XMFLOAT3 PlayerPos = m_pPlayer->GetPosition();

		float Distance = Vector3::Length(Vector3::Subtract(ObjPos, PlayerPos));
		if (Distance < 10)
			dynamic_cast<CItemBox*>(pObj)->Open();
	}
}

CScene::CScene()
{
	m_pd3dGraphicsRootSignature = NULL;
	m_fElapsedTime = 0.0f;
}

CScene::~CScene()
{
	//if (m_pd3dGraphicsRootSignature)
	//	m_pd3dGraphicsRootSignature->Release();
}

