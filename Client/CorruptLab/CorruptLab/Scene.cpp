//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "Object_Terrain.h"
#include "Shader_Water.h"

CGameScene::CGameScene()
{
}

CGameScene::~CGameScene()
{
}

void CGameScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	std::cout << "누나 졸작 화이팅 by 은우" << std::endl;
	XMFLOAT3 xmf3Scale(2.0f, 0.5f, 2.0f);
	XMFLOAT4 xmf4Color(0.6f, 0.5f, 0.2f, 0.0f);

	m_pSkyBox = new CSkyBox(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature,
		                                   _T("Geometry/terrain2.raw"),257, 257, 9, 9, xmf3Scale, xmf4Color);

	m_pCloudGSShader = new CCloudGSShader; 
	m_pCloudGSShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature, 3);
	m_pCloudGSShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pTerrain);

	m_pWaterShader = new CWaterShader;
	m_pWaterShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature, 3);
	m_pWaterShader->BuildObjects(pd3dDevice, pd3dCommandList);

	m_pNoiseObject = new CObjectNosie(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	m_pNoiseObject->SetPosition(XMFLOAT3(458.0f, 55.0f, 198.0f));
	m_pNoiseObject->GenerateShaderDistortionBuffer();

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CGameScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature)
		m_pd3dGraphicsRootSignature->Release();

	ReleaseShaderVariables();

	if (m_pSkyBox)        delete m_pSkyBox;
	if (m_pTerrain)       delete m_pTerrain;
	if (m_pCloudGSShader) delete m_pCloudGSShader; 


	if (m_pObjectLists) // 오브젝트 Release
	{
		for (int i = 0; i < m_nObjectTypeNum; i++)
		{
			for (auto Obj : *m_pObjectLists[i])
			{
				Obj->Release();
			}
			m_pObjectLists[i]->clear();
		}
	}
	delete[] m_pObjectLists;

}

void CGameScene::ReleaseUploadBuffers()
{
	if (m_pSkyBox) m_pSkyBox->ReleaseUploadBuffers();
	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();
}

void CGameScene::SetTerrainPipelineState()
{
	m_pTerrain->SetPipelinemode();
}

ID3D12RootSignature *CGameScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_DESCRIPTOR_RANGE pd3dMaterialTexRanges[7];

	pd3dMaterialTexRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[0].NumDescriptors = 1;
	pd3dMaterialTexRanges[0].BaseShaderRegister = 4; //t4: gtxtAlbedoTexture
	pd3dMaterialTexRanges[0].RegisterSpace = 0;
	pd3dMaterialTexRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[1].NumDescriptors = 1;
	pd3dMaterialTexRanges[1].BaseShaderRegister = 5; //t7: gtxtSpecularTexture
	pd3dMaterialTexRanges[1].RegisterSpace = 0;
	pd3dMaterialTexRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[2].NumDescriptors = 1;
	pd3dMaterialTexRanges[2].BaseShaderRegister = 6; //t8: gtxtNormalTexture
	pd3dMaterialTexRanges[2].RegisterSpace = 0;
	pd3dMaterialTexRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[3].NumDescriptors = 1;
	pd3dMaterialTexRanges[3].BaseShaderRegister = 7; //t9: gtxtMetallicTexture
	pd3dMaterialTexRanges[3].RegisterSpace = 0;
	pd3dMaterialTexRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[4].NumDescriptors = 1;
	pd3dMaterialTexRanges[4].BaseShaderRegister = 8; //t10: gtxtEmissionTexture
	pd3dMaterialTexRanges[4].RegisterSpace = 0;
	pd3dMaterialTexRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[5].NumDescriptors = 1;
	pd3dMaterialTexRanges[5].BaseShaderRegister = 9; //t10: gtxtEmissionTexture
	pd3dMaterialTexRanges[5].RegisterSpace = 0;
	pd3dMaterialTexRanges[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dMaterialTexRanges[6].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMaterialTexRanges[6].NumDescriptors = 1;
	pd3dMaterialTexRanges[6].BaseShaderRegister = 10; //t10: gtxtEmissionTexture
	pd3dMaterialTexRanges[6].RegisterSpace = 0;
	pd3dMaterialTexRanges[6].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dSkyTexRange; 
	pd3dSkyTexRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dSkyTexRange.NumDescriptors = 1;
	pd3dSkyTexRange.BaseShaderRegister = 11; // t13:gtxtSkyCubeTexture
	pd3dSkyTexRange.RegisterSpace = 0;
	pd3dSkyTexRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dCloudTexRange;
	pd3dCloudTexRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dCloudTexRange.NumDescriptors = 1;
	pd3dCloudTexRange.BaseShaderRegister = 12; // t14:gtxtCloudTexture
	pd3dCloudTexRange.RegisterSpace = 0;
	pd3dCloudTexRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dTerrainTexRanges;
	pd3dTerrainTexRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dTerrainTexRanges.NumDescriptors = 6 ;
	pd3dTerrainTexRanges.BaseShaderRegister = 13; 
	//t11: gtxtTerrainBaseTexture  //t12: gtxtTerrainAlaphTexture //t13 Stone1_Bc //t14 DryStone_BC 
	pd3dTerrainTexRanges.RegisterSpace = 0;
	pd3dTerrainTexRanges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	
	D3D12_DESCRIPTOR_RANGE pd3dWaterTexRanges;
	pd3dWaterTexRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dWaterTexRanges.NumDescriptors = 1;
	pd3dWaterTexRanges.BaseShaderRegister = 24;
	pd3dWaterTexRanges.RegisterSpace = 0;
	pd3dWaterTexRanges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE pd3dNoiseTexRanges;
	pd3dNoiseTexRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dNoiseTexRanges.NumDescriptors = 3;
	pd3dNoiseTexRanges.BaseShaderRegister = 19;
	pd3dNoiseTexRanges.RegisterSpace = 0;
	pd3dNoiseTexRanges.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER pd3dRootParameters[18];

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

	pd3dRootParameters[ROOT_PARAMETER_DETAIL_ALEBEDO_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_DETAIL_ALEBEDO_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_DETAIL_ALEBEDO_TEX].DescriptorTable.pDescriptorRanges = &pd3dMaterialTexRanges[5];
	pd3dRootParameters[ROOT_PARAMETER_DETAIL_ALEBEDO_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	pd3dRootParameters[ROOT_PARAMETER_DETAIL_NORMAL_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_DETAIL_NORMAL_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_DETAIL_NORMAL_TEX].DescriptorTable.pDescriptorRanges = &pd3dMaterialTexRanges[6];
	pd3dRootParameters[ROOT_PARAMETER_DETAIL_NORMAL_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

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
	pd3dRootParameters[ROOT_PARAMETER_TERRAIN_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARMAMETER_WATER_NORMAL_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARMAMETER_WATER_NORMAL_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARMAMETER_WATER_NORMAL_TEX].DescriptorTable.pDescriptorRanges = &pd3dWaterTexRanges;
	pd3dRootParameters[ROOT_PARMAMETER_WATER_NORMAL_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_NOISEBUFFER].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_NOISEBUFFER].Constants.ShaderRegister = 22;
	pd3dRootParameters[ROOT_PARAMETER_NOISEBUFFER].Constants.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_NOISEBUFFER].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_DISTORTIONBUFFER].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_DISTORTIONBUFFER].Constants.ShaderRegister = 23;
	pd3dRootParameters[ROOT_PARAMETER_DISTORTIONBUFFER].Constants.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_DISTORTIONBUFFER].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_NOISE_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_NOISE_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_NOISE_TEX].DescriptorTable.pDescriptorRanges = &pd3dNoiseTexRanges;
	pd3dRootParameters[ROOT_PARAMETER_NOISE_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

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
	d3dSamplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

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
	//D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = _countof(d3dSamplerDesc);
	d3dRootSignatureDesc.pStaticSamplers = d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);

	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);

}

void CGameScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	
}

void CGameScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
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
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
			SetTerrainPipelineState();
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
				m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
			else
				m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
		}
		if (dwDirection)
			m_pPlayer->Move(dwDirection, 0.5f, true);
	}
	m_pPlayer->Update(m_fElapsedTime);

	return true;
}

void CGameScene::AnimateObjects(float fTimeElapsed)
{
}

void CGameScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	pd3dCommandList->OMSetStencilRef(1);
	if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);

	if (m_pObjectLists) // 오브젝트 Render
	{
		for (int i = 0; i < m_nObjectTypeNum; i++)
		{
			for (auto Obj : *m_pObjectLists[i])
			{
				Obj->UpdateTransform(NULL);
				Obj->Render(pd3dCommandList, pCamera,0);
			}
		}
	}

	if (m_pCloudGSShader) m_pCloudGSShader->Render(pd3dCommandList, pCamera); 
	if (m_pNoiseObject) m_pNoiseObject->Render(pd3dCommandList, pCamera); 
	m_pPlayer->Render(pd3dCommandList, pCamera);
	
	
	if(m_pWaterShader) m_pWaterShader->Render(pd3dCommandList, pCamera);
	if (m_pSkyBox) m_pSkyBox->Render(pd3dCommandList, pCamera);

}


void CGameScene::Update(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
	AnimateObjects(fTimeElapsed);

	m_pPlayer->Animate(fTimeElapsed, NULL);
}

