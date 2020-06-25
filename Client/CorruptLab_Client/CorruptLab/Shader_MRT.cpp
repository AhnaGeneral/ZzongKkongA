#include "Shader_MRT.h"
#include "Shader_BaseUI.h"
#include "Shader_Item.h"
#include "Shader_Radiation.h"
#include "Mgr_Radiation.h"
#include "Mgr_Item.h"
#include "Shader_MinmapFog.h"

CPostProcessingShader::CPostProcessingShader()
{
	m_xmf4x4Ortho = Matrix4x4::Identity();
	m_xmf4x4OrthoView = Matrix4x4::Identity();

	m_pTexture = NULL;
	m_pLightTexture = NULL;
	m_pShadowTexture = NULL;
	m_pItemTex = NULL;
	m_pBaseUIShader = NULL;
	m_pMinimapFog = NULL;
	m_pItemShader = NULL;

	m_pRenderTargetUIs = NULL;
	m_nRenderTargetUI = 0;
	pRenderTargetUI = NULL;
	m_nMRTSwitch = 1;

	m_pMinimap = NULL;

	m_pHP = NULL;

	m_Radiation = NULL;

	m_RadiationLevels = NULL;
    m_RadiationCount = NULL;

	m_pRadiationShader = NULL;

	m_HPBAR = NULL;

	m_ppInVentoryBoxs = NULL;
	m_pInventoryBox = NULL;

	m_ppItems = NULL;
	m_pItem = NULL;
	m_PlayerHP = NULL;

	nIventoryCount = 3;

	m_pd3dcbvOrthoCamera = NULL;
	m_pcbMappedOrthoCamera = NULL;

}

CPostProcessingShader::~CPostProcessingShader()
{
	ReleaseShaderVariables();
	ReleaseObjects();
}

D3D12_DEPTH_STENCIL_DESC CPostProcessingShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return(d3dDepthStencilDesc);
}

D3D12_INPUT_LAYOUT_DESC CPostProcessingShader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = NULL;
	d3dInputLayoutDesc.NumElements = 0;

	return(d3dInputLayoutDesc);
}

void CPostProcessingShader::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[3];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 6;
	pd3dDescriptorRanges[0].BaseShaderRegister = 1; //Texture[]
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = 0;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 21; //LightTexture
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = 0;

	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 24; //ShadowMap
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = 0;

	D3D12_DESCRIPTOR_RANGE pd3dHPTextureRanges;
	pd3dHPTextureRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dHPTextureRanges.NumDescriptors = 1;
	pd3dHPTextureRanges.BaseShaderRegister = 26;
	pd3dHPTextureRanges.RegisterSpace = 0;
	pd3dHPTextureRanges.OffsetInDescriptorsFromTableStart = 0;

	D3D12_DESCRIPTOR_RANGE pd3dItemTextureRanges;
	pd3dItemTextureRanges.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dItemTextureRanges.NumDescriptors = 3;
	pd3dItemTextureRanges.BaseShaderRegister = 27;
	pd3dItemTextureRanges.RegisterSpace = 0;
	pd3dItemTextureRanges.OffsetInDescriptorsFromTableStart = 0;

	D3D12_DESCRIPTOR_RANGE pd3dMinimapFog;
	pd3dMinimapFog.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dMinimapFog.NumDescriptors = 1;
	pd3dMinimapFog.BaseShaderRegister = 30;
	pd3dMinimapFog.RegisterSpace = 0;
	pd3dMinimapFog.OffsetInDescriptorsFromTableStart = 0;

	D3D12_ROOT_PARAMETER pd3dRootParameters[11];

	pd3dRootParameters[ROOT_PARAMETER_CDN_MRT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_CDN_MRT].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_CDN_MRT].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0]; //Texture
	pd3dRootParameters[ROOT_PARAMETER_CDN_MRT].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_OBJECT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[ROOT_PARAMETER_OBJECT].Constants.Num32BitValues = 32;
	pd3dRootParameters[ROOT_PARAMETER_OBJECT].Constants.ShaderRegister = 2; //b2 : GameObject
	pd3dRootParameters[ROOT_PARAMETER_OBJECT].Constants.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_OBJECT].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_ORTHO].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_ORTHO].Descriptor.ShaderRegister = 4; //b1 : Camera
	pd3dRootParameters[ROOT_PARAMETER_ORTHO].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_ORTHO].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_LIGHT_MRT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_LIGHT_MRT].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_LIGHT_MRT].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1]; //LightTexture
	pd3dRootParameters[ROOT_PARAMETER_LIGHT_MRT].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_SHADOW_MRT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_SHADOW_MRT].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_SHADOW_MRT].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[2]; //LightTexture
	pd3dRootParameters[ROOT_PARAMETER_SHADOW_MRT].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_PLAYER_POS].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_PLAYER_POS].Descriptor.ShaderRegister = 6; //playerPosition
	pd3dRootParameters[ROOT_PARAMETER_PLAYER_POS].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_PLAYER_POS].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_HP_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_HP_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_HP_TEX].DescriptorTable.pDescriptorRanges = &pd3dHPTextureRanges;
	pd3dRootParameters[ROOT_PARAMETER_HP_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_ITEM_TEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_ITEM_TEX].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_ITEM_TEX].DescriptorTable.pDescriptorRanges = &pd3dItemTextureRanges;
	pd3dRootParameters[ROOT_PARAMETER_ITEM_TEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[ROOT_PARAMETER_ITEM_REACT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_ITEM_REACT].Descriptor.ShaderRegister = 7; //playerPosition
	pd3dRootParameters[ROOT_PARAMETER_ITEM_REACT].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_ITEM_REACT].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_PLAYER_HP].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[ROOT_PARAMETER_PLAYER_HP].Descriptor.ShaderRegister = 8; //playerHP
	pd3dRootParameters[ROOT_PARAMETER_PLAYER_HP].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[ROOT_PARAMETER_PLAYER_HP].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[ROOT_PARAMETER_MINIIMAPFOG].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[ROOT_PARAMETER_MINIIMAPFOG].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[ROOT_PARAMETER_MINIIMAPFOG].DescriptorTable.pDescriptorRanges = &pd3dMinimapFog;
	pd3dRootParameters[ROOT_PARAMETER_MINIIMAPFOG].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;




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

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | 
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS ;

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
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(),
		__uuidof(ID3D12RootSignature), (void**)&m_pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();
}

D3D12_SHADER_BYTECODE CPostProcessingShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"HLSL_MRT.hlsl", "VSPostProcessing", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPostProcessingShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"HLSL_MRT.hlsl", "PSPostProcessing", "ps_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPostProcessingShader::CreateIndoorPixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"HLSL_MRT.hlsl", "PSIndoorPostProcessing", "ps_5_1", ppd3dShaderBlob));
}

void CPostProcessingShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, UINT nRenderTargets)
{
	m_nPipelineStates = 3;
	m_ppd3dPipelineStates = new ID3D12PipelineState * [m_nPipelineStates];
	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = nRenderTargets;

	for (UINT i = 0; i < nRenderTargets; i++)
		d3dPipelineStateDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;

	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[0]);

	d3dPipelineStateDesc.PS = CreateIndoorPixelShader(&pd3dPixelShaderBlob);
	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[1]);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	//----------------------------------------------------------------------


	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	d3dPipelineStateDesc.InputLayout = d3dInputLayoutDesc;
	d3dPipelineStateDesc.VS = CShader::CompileShaderFromFile(L"HLSL_MRT.hlsl", "VSUI", "vs_5_1", &pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CShader::CompileShaderFromFile(L"HLSL_MRT.hlsl", "PSUI", "ps_5_1", &pd3dPixelShaderBlob);

	hResult = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_ppd3dPipelineStates[2]);

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CPostProcessingShader::SetRenderTargets(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
	void* pContext, void* pLightContext, void* pShadowContext)
{
	if (pContext != NULL)
	{
		m_pTexture = (CTexture*)pContext;
		CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 8);
		CreateShaderVariables(pd3dDevice, pd3dCommandList);
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pTexture, ROOT_PARAMETER_CDN_MRT, true);
	}

	if (pLightContext != NULL)
	{
		m_pLightTexture = (CTexture*)pLightContext;
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pLightTexture, ROOT_PARAMETER_LIGHT_MRT, 0);
	}

	if (pShadowContext != NULL)
	{
		m_pShadowTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
		m_pShadowTexture->SetTexture(0, ((CTexture*)pShadowContext)->GetTexture(0));
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pShadowTexture, ROOT_PARAMETER_SHADOW_MRT, 0);
	}


}

void CPostProcessingShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_xmf4x4OrthoView =
		Matrix4x4::LookAtLH(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	// position�� �� 0 �̿��� ���� ? // ��� �����̿��� �ϴ°� �ƴѰ� ? 
	GenerateOrthoLHMatrix(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);

	// [ ���߷���Ÿ�� �ؽ��� ] ===============================================================================
	CTriangleRect* mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, FRAME_BUFFER_WIDTH / 7.0f, FRAME_BUFFER_HEIGHT / 7.f, 0.0f, 1.0f);
	m_nRenderTargetUI = 7;
	m_pRenderTargetUIs = new CGameObject * [m_nRenderTargetUI];
	for (int i = 0; i < m_nRenderTargetUI;)
	{
		int remainder = i % 5;
		int tmp = i / 5;
		pRenderTargetUI = new CMRTUI(pd3dDevice, pd3dCommandList);
		pRenderTargetUI->SetMesh(mesh);
		pRenderTargetUI->Set2DPosition(-500.0f + (remainder * 180.0f), 340.0f + (-tmp * 120));
		pRenderTargetUI->SetObjectID(i);
		m_pRenderTargetUIs[i++] = pRenderTargetUI;
	}

	//[ �̴ϸ� ] ============================================================================================
	m_pMinimap = new CUI_MiniMap(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
	m_pMinimap->InterLinkShaderTexture(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
	mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 180, 180, 0.0f, 1.0f);
	m_pMinimap->SetMesh(mesh);
	m_pMinimap->Set2DPosition((+FRAME_BUFFER_WIDTH / 2) - 90, (-FRAME_BUFFER_HEIGHT / 2) + 90);

	//[ ü�� ] ==============================================================================================
	CTexture* m_pHPTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pHPTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/HP/HP_3.dds", 0);

	m_pBaseUIShader = new CShader_BaseUI();
	m_pBaseUIShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 4);
	m_pBaseUIShader->CreateShader(pd3dDevice, GetGraphicsRootSignature(), FINAL_MRT_COUNT);
	m_pBaseUIShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)m_pHPTexture, ROOT_PARAMETER_HP_TEX, true);

	//float m_Alpha = 1; 
	m_HPBAR = new CUI_Root(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
	m_HPBAR->InterLinkShaderTexture(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature(), NULL, m_pHPTexture);
	mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 250, 200, 0.0f, 1.0f);
	m_HPBAR->Set2DPosition((-FRAME_BUFFER_WIDTH / 2) + 125, (-FRAME_BUFFER_HEIGHT / 2) + 100);
	m_HPBAR->SetMesh(mesh);	
	m_HPBAR->SetAlpha(&m_Alpha);

	// ========================================================================================================
	CTexture* m_pIndoorGreetingTex = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pIndoorGreetingTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Font/IndoorGreeting.dds", 0);
	m_pBaseUIShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)m_pIndoorGreetingTex, ROOT_PARAMETER_HP_TEX, true);

	m_IndoorGreeting = new CUI_Root(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
	m_IndoorGreeting->InterLinkShaderTexture(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature(), NULL, m_pIndoorGreetingTex);
	mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 512, 256, 0.0f, 1.0f);
	m_IndoorGreeting->Set2DPosition((-FRAME_BUFFER_WIDTH / 2) + 380, (-FRAME_BUFFER_HEIGHT / 2) + 500);
	m_IndoorGreeting->SetMesh(mesh);
	m_IndoorGreeting->SetAlpha(&m_Alpha);

	//[ ���� �ڽ� ] ===========================================================================================
	CTexture* pRadiationTex = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pRadiationTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/HP/HP_1.dds", 0);

	m_pBaseUIShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)pRadiationTex, ROOT_PARAMETER_HP_TEX, true);

	m_Radiation = new CUI_Root(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
	m_Radiation->InterLinkShaderTexture(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature(), NULL, pRadiationTex);
	mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 250, 200, 0.0f, 1.0f);
	m_Radiation->Set2DPosition((-FRAME_BUFFER_WIDTH / 2) + 125, (-FRAME_BUFFER_HEIGHT / 2) + 100);
	m_Radiation->SetMesh(mesh);
	m_Radiation->SetAlpha(&m_Alpha);


	// ========================================================================================================
	m_pMinimapFog = new CShader_MinmapFog();
	m_pMinimapFog->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 2);
	m_pMinimapFog->CreateShader(pd3dDevice, GetGraphicsRootSignature(), FINAL_MRT_COUNT);

	pMinmapFog1 = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pMinmapFog1->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/MiniMap/Map_Fog1.dds", 0);

	m_pMinimapFog->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)pMinmapFog1, ROOT_PARAMETER_MINIIMAPFOG, true);

	m_pMapOne = new CUI_ITem(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
	mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 180, 180, 0.0f, 1.0f);
	m_pMapOne->SetObjectID(0);
	m_pMapOne->Set2DPosition((+FRAME_BUFFER_WIDTH / 2) - 90, (-FRAME_BUFFER_HEIGHT / 2) + 90);
	m_pMapOne->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_pMapOne->SetMesh(mesh);

    pMinmapFog2 = new CTexture(1, RESOURCE_TEXTURE2D, 0);

	pMinmapFog2->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/MiniMap/Map_Fog2.dds", 0);

	m_pMinimapFog->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)pMinmapFog2, ROOT_PARAMETER_MINIIMAPFOG, true);

	m_pMapTwo = new CUI_ITem(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
	mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 180, 180, 0.0f, 1.0f);
	m_pMapTwo->SetObjectID(1);
	m_pMapTwo->Set2DPosition((+FRAME_BUFFER_WIDTH / 2) - 90, (-FRAME_BUFFER_HEIGHT / 2) + 90);
	m_pMapTwo->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_pMapTwo->SetMesh(mesh);



	//[ �κ��丮 ] ==============================================================================================
	m_ppInVentoryBoxs = new CGameObject * [nIventoryCount];

	float ItemBoxSize = FRAME_BUFFER_HEIGHT / 10.0f;

	CTexture* pInventoryTex = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pInventoryTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Inventory/Inventory.dds", 0);

	m_pBaseUIShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)pInventoryTex, ROOT_PARAMETER_HP_TEX, true);

	for (int i = 0; i < (int)nIventoryCount;)
	{
		m_pInventoryBox = new CUI_Root(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
		m_pInventoryBox->InterLinkShaderTexture(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature(), NULL, pInventoryTex);
		mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, ItemBoxSize, ItemBoxSize, 0.0f, 1.0f);
		m_pInventoryBox->SetMesh(mesh);
		m_pInventoryBox->SetAlpha(&m_Alpha);

		m_pInventoryBox->Set2DPosition((-FRAME_BUFFER_WIDTH / 2) + 30 + (i * ItemBoxSize), (-FRAME_BUFFER_HEIGHT / 2) + 30);
		m_ppInVentoryBoxs[i++] = m_pInventoryBox;
	}

	//[ �����۾� ] =============================================================================================
	m_ppItems = new CGameObject * [nIventoryCount];

	m_pItemTex = new CTexture(3, RESOURCE_TEXTURE2D, 0);
    m_pItemTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Inventory/HandLight.dds", 0);
    m_pItemTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Inventory/HP_Kit.dds", 1);
    m_pItemTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/Inventory/Pill.dds", 2);

	//m_pBaseUIShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)pItemTex, ROOT_PARAMETER_HP_TEX, true);

	m_pItemShader = new CShader_Item();
	m_pItemShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 3);
	m_pItemShader->CreateShader(pd3dDevice, GetGraphicsRootSignature(), FINAL_MRT_COUNT);
    m_pItemShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, m_pItemTex, ROOT_PARAMETER_ITEM_TEX, true);

	for (int i = 0; i < int(nIventoryCount);)
	{
		m_pItem = new CUI_ITem(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
		m_pItem->InterLinkShaderTexture(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature(), NULL);

		mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, ItemBoxSize, ItemBoxSize, 0.0f, 1.0f); //60, 60
		m_pItem->CreateShaderVariables(pd3dDevice, pd3dCommandList);
		m_pItem->SetMesh(mesh);
		m_pItem->SetObjectID(i);
		//UINT unber = 1;
		//m_pItem->SetItemReact(&unber);
		m_pItem->Set2DPosition((-FRAME_BUFFER_WIDTH / 2) + 30 + (i * ItemBoxSize), (-FRAME_BUFFER_HEIGHT / 2) + 30);
		m_ppItems[i++] = m_pItem;
	}
	//[ ü�¹� ] =================================================================================================
	CTexture* pHPTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pHPTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/HP/HP_2.dds", 0);

	m_PlayerHP = new CUI_HP(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
	m_PlayerHP->InterLinkShaderTexture(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature(), pHPTexture);
	mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 190, 18, 0.0f, 1.0f);
	m_PlayerHP->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	m_PlayerHP->Set2DPosition((-FRAME_BUFFER_WIDTH / 2) + 150, (-FRAME_BUFFER_HEIGHT / 2) + 90);
	m_PlayerHP->SetObjectID(4);
	m_PlayerHP->SetMesh(mesh);

	//[ ���� ��ġ ī���� ] ======================================================================================= 
	CTexture* pRadiationCountTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pRadiationCountTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/RadiationCount.dds", 0);

	m_pRadiationShader = new CShader_Radiation();
	m_pRadiationShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 1);
	m_pRadiationShader->CreateShader(pd3dDevice, GetGraphicsRootSignature(), FINAL_MRT_COUNT);
	m_pRadiationShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, (CTexture*)pRadiationCountTexture, ROOT_PARAMETER_HP_TEX, true);

	m_RadiationLevels = new CGameObject*[2];
	for (int i = 0; i < 2;)
	{
		m_RadiationCount = new CUI_RaditaionLevel(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature());
		m_RadiationCount->InterLinkShaderTexture(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature(), NULL, pRadiationCountTexture);
		m_RadiationCount->CreateShaderVariables(pd3dDevice, pd3dCommandList);

		mesh = new CTriangleRect(pd3dDevice, pd3dCommandList, 30, 40, 0.0f, 1.0f);

		m_RadiationCount->Set2DPosition(float((-FRAME_BUFFER_WIDTH / 2) + 25 *(i+1)), float((-FRAME_BUFFER_HEIGHT / 2) + 105));
		m_RadiationCount->SetMesh(mesh);
		m_RadiationCount->SetRadiationNumber(9);
		m_RadiationLevels[i++] = m_RadiationCount;
	}
	// =============================================================================================================

	ReleaseUploadBuffers();
}

void CPostProcessingShader::ReleaseObjects()
{
	if (m_pTexture)
	{
		m_pTexture->ReleaseShaderVariables();
		m_pTexture->ReleaseUploadBuffers();
		m_pTexture->Release(); 
		m_pTexture = NULL;
	}

	if (m_pLightTexture)
	{
		m_pLightTexture->ReleaseShaderVariables();
		m_pLightTexture->ReleaseUploadBuffers();
		m_pLightTexture->Release();
		m_pLightTexture = NULL;
	}
	if (m_pShadowTexture)
	{
		m_pShadowTexture->ReleaseShaderVariables();
		m_pShadowTexture->ReleaseUploadBuffers();
		m_pShadowTexture->Release();
		m_pShadowTexture = NULL;
	}

	if (m_pItemTex)
	{
		m_pItemTex->ReleaseShaderVariables();
		m_pItemTex->ReleaseUploadBuffers();
		m_pItemTex->Release();
		m_pItemTex = NULL;
	}

	if (pMinmapFog1)
	{
		pMinmapFog1->ReleaseShaderVariables();
		pMinmapFog1->ReleaseUploadBuffers();
		pMinmapFog1->Release();
		pMinmapFog1 = NULL;
	}
	if (pMinmapFog2)
	{
		pMinmapFog2->ReleaseShaderVariables();
		pMinmapFog2->ReleaseUploadBuffers();
		pMinmapFog2->Release();
		pMinmapFog2 = NULL;
	}

	if (m_pBaseUIShader)
	{
		m_pBaseUIShader->ReleaseShaderVariables();
		m_pBaseUIShader->ReleaseUploadBuffers();
		m_pBaseUIShader->Release();
		m_pBaseUIShader = NULL;
	}

	if (m_pMinimapFog)
	{
		m_pMinimapFog->ReleaseShaderVariables();
		m_pMinimapFog->ReleaseUploadBuffers();
		m_pMinimapFog->Release();
		m_pMinimapFog = NULL;
	}
	if (m_pItemShader)
	{
		m_pItemShader->ReleaseShaderVariables();
		m_pItemShader->ReleaseUploadBuffers();
		m_pItemShader->Release();
		m_pItemShader = NULL;
	}

	if (m_pRenderTargetUIs)
	{
		for (int i = 0; i < m_nRenderTargetUI; ++i)
		{
			m_pRenderTargetUIs[i]->ReleaseShaderVariables();
			m_pRenderTargetUIs[i]->ReleaseUploadBuffers();
			m_pRenderTargetUIs[i]->Release();
			m_pRenderTargetUIs[i] = NULL;
		}
		m_pRenderTargetUIs = NULL;
	}

	if (m_pHP)
	{
		m_pHP->ReleaseShaderVariables();
		m_pHP->ReleaseUploadBuffers();
		m_pHP->Release();
		m_pHP = NULL;
	}
	if (m_Radiation)
	{
		m_Radiation->ReleaseShaderVariables();
		m_Radiation->ReleaseUploadBuffers();
		m_Radiation->Release();
		m_Radiation = NULL;
	}

	if (m_IndoorGreeting)
	{
		m_IndoorGreeting->ReleaseShaderVariables();
		m_IndoorGreeting->ReleaseUploadBuffers();
		m_IndoorGreeting->Release();
		m_IndoorGreeting = NULL;
	}
	

	if (m_pMapOne)
	{
		m_pMapOne->ReleaseShaderVariables();
		m_pMapOne->ReleaseUploadBuffers();
		m_pMapOne->Release();
		m_pMapOne = NULL;
	}
	if (m_pMapTwo)
	{
		m_pMapTwo->ReleaseShaderVariables();
		m_pMapTwo->ReleaseUploadBuffers();
		m_pMapTwo->Release();
		m_pMapTwo = NULL;
	}

	if (m_pMinimap)
	{
		m_pMinimap->ReleaseShaderVariables();
		m_pMinimap->ReleaseUploadBuffers();
		m_pMinimap->Release();
		m_pMinimap = NULL;
	}
	if (m_pRadiationShader)
	{
		m_pRadiationShader->ReleaseShaderVariables();
		m_pRadiationShader->ReleaseUploadBuffers();
		m_pRadiationShader->Release();
		m_pRadiationShader = NULL;
	}

	if (m_RadiationLevels)
	{
		for (int i = 0; i < 2; ++i)
		{
			m_RadiationLevels[i]->ReleaseShaderVariables();
			m_RadiationLevels[i]->ReleaseUploadBuffers();
			m_RadiationLevels[i]->Release();
			m_RadiationLevels[i] = NULL;
		}
		m_RadiationLevels = NULL;
	}

	if (m_HPBAR)
	{
		m_HPBAR->ReleaseShaderVariables();
		m_HPBAR->ReleaseUploadBuffers();
		m_HPBAR->Release();
		m_HPBAR = NULL;
	}

	if (m_ppInVentoryBoxs)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_ppInVentoryBoxs[i]->ReleaseShaderVariables();
			m_ppInVentoryBoxs[i]->ReleaseUploadBuffers();
			m_ppInVentoryBoxs[i]->Release();
			m_ppInVentoryBoxs[i] = NULL; 
		}
	}

	if (m_ppItems)
	{
		for (int i = 0; i < nIventoryCount; ++i)
		{
			m_ppItems[i]->ReleaseShaderVariables();
			m_ppItems[i]->ReleaseUploadBuffers();
			m_ppItems[i]->Release();
			m_ppItems[i] = NULL;
		}
	}
	if (m_PlayerHP)
	{
		m_PlayerHP->ReleaseShaderVariables();
		m_PlayerHP->ReleaseUploadBuffers();
		m_PlayerHP->Release();
		m_PlayerHP = NULL;
	}

}

void CPostProcessingShader::ReleaseUploadBuffers()
{
}

void CPostProcessingShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int npipelinestate)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);

	if (m_pd3dGraphicsRootSignature)
		pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	if (m_ppd3dPipelineStates)
		pd3dCommandList->SetPipelineState(m_ppd3dPipelineStates[npipelinestate]);

	if (m_pTexture) m_pTexture->UpdateShaderVariables(pd3dCommandList);

	if (m_pd3dCbvSrvDescriptorHeap)
		pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);


	if (m_pLightTexture) m_pLightTexture->UpdateShaderVariables(pd3dCommandList);
	if (m_pShadowTexture) m_pShadowTexture->UpdateShaderVariables(pd3dCommandList);

	pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dCommandList->DrawInstanced(6, 1, 0, 0);

	OnPrepareRender(pd3dCommandList, 2);

	if (m_nMRTSwitch)
	{
		for (int i = 0; i < m_nRenderTargetUI; i++)
		{
			if (m_pRenderTargetUIs[i])
			{
				m_pRenderTargetUIs[i]->Render(pd3dCommandList, 0);
			}
		}
	}
	UIRender(pd3dCommandList, pCamera, npipelinestate);
}

void CPostProcessingShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// [ �������� ] --------------------------------------------------------------------------------
	UINT ncbElementBytes = ((sizeof(VS_CB_EYE_CAMERA_ORTHO) + 255) & ~255); //256�� ���  
	m_pd3dcbvOrthoCamera = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, 
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pd3dcbvOrthoCamera->Map(0, NULL, (void**)&m_pcbMappedOrthoCamera);
}

void CPostProcessingShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	// [ �������� ] --------------------------------------------------------------------------------
	XMStoreFloat4x4(&m_pcbMappedOrthoCamera->m_xmf4x4Ortho, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Ortho)));
	XMStoreFloat4x4(&m_pcbMappedOrthoCamera->m_xmf4x4OrthoView, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4OrthoView)));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbvOrthoCamera->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(ROOT_PARAMETER_ORTHO, d3dGpuVirtualAddress);
}

void CPostProcessingShader::ReleaseShaderVariables()
{
	// [ �������� ] --------------------------------------------------------------------------------
	if (m_pd3dcbvOrthoCamera)
	{
		m_pd3dcbvOrthoCamera->Unmap(0, NULL);
		m_pd3dcbvOrthoCamera->Release();
	}
	// --------------------------------------------------------------------------------------------
	
	CRadationMgr::GetInstance()->Destroy();
	
	if (m_pTexture) m_pTexture->ReleaseShaderVariables();
	if (m_pLightTexture) m_pLightTexture->ReleaseShaderVariables();
	if (m_pMinimap) m_pMinimap->ReleaseShaderVariables();
	if (m_pShadowTexture) m_pShadowTexture->ReleaseShaderVariables();
	if (m_pItemTex) m_pItemTex->ReleaseShaderVariables();

	if (m_pRenderTargetUIs)
	{
		for (int i = 0; i < m_nRenderTargetUI; ++i)
			m_pRenderTargetUIs[i]->ReleaseShaderVariables();
	}

	if (m_pHP) m_pHP->ReleaseShaderVariables();
	if (m_Radiation) m_Radiation->ReleaseShaderVariables();
	if (m_HPBAR) m_HPBAR->ReleaseShaderVariables();

	if (m_ppInVentoryBoxs)
	{
		for (int i = 0; i < 3; ++i)
			m_ppInVentoryBoxs[i]->ReleaseShaderVariables();
	}

	if (m_ppItems)
	{
		for (int i = 0; i < 3; ++i)
			m_ppItems[i]->ReleaseShaderVariables();
	}
	if (m_PlayerHP) m_PlayerHP->ReleaseShaderVariables();


}

void CPostProcessingShader::UIRender(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int npipelinestate)
{
	
	if (m_pBaseUIShader)m_pBaseUIShader->Render(pd3dCommandList, pCamera);

	if (m_HPBAR) m_HPBAR->Render(pd3dCommandList, pCamera);

	if ((npipelinestate == 1))
	{
		//if (m_IndoorGreeting) m_IndoorGreeting->Render(pd3dCommandList, pCamera);
	}


	if (m_Radiation) m_Radiation->Render(pd3dCommandList, pCamera);

	for (int i = 0; i < int(nIventoryCount); ++i)
	{
		m_ppInVentoryBoxs[i]->Render(pd3dCommandList, 0);
	}

	if (m_pItemShader)m_pItemShader->Render(pd3dCommandList, pCamera);
	if (m_pItemTex) m_pItemTex->UpdateShaderVariable(pd3dCommandList, 0);

	for (int i = 0; i < int(nIventoryCount); ++i)
	{
		dynamic_cast<CUI_ITem*>(m_ppItems[i])->SetItemCount(CItemMgr::GetInstance()->GetItemNums());
		dynamic_cast<CUI_ITem*>(m_ppItems[i])->SetItemReact(CItemMgr::GetInstance()->GetReactIten());
		m_ppItems[i]->Render(pd3dCommandList, 0);
	}

	if (m_PlayerHP) m_PlayerHP->Render(pd3dCommandList, pCamera);


	// PipelineState = 1 [IndoorState] ==================================================== 
	if (!(npipelinestate == 1))
	{
		if (m_pMinimap) m_pMinimap->Render(pd3dCommandList, pCamera);
		if (m_pRadiationShader) m_pRadiationShader->Render(pd3dCommandList, pCamera);

		if (m_RadiationLevels)
		{
			int number = CRadationMgr::GetInstance()->GetRaditaion();
			dynamic_cast<CUI_RaditaionLevel*>(m_RadiationLevels[0])->SetRadiationNumber(int(number / 10));
			dynamic_cast<CUI_RaditaionLevel*>(m_RadiationLevels[1])->SetRadiationNumber(int(number % 10));

			for (int i = 0; i < 2; ++i)
			{
				m_RadiationLevels[i]->Render(pd3dCommandList, pCamera);
			}
		}


		if (m_pMinimapFog)m_pMinimapFog->Render(pd3dCommandList, pCamera);

		if (pMinmapFog1) pMinmapFog1->UpdateShaderVariable(pd3dCommandList, 0);

		if (m_pMapOne)
		{
			(m_pMapOne)->SetItemCount(CItemMgr::GetInstance()->GetItemNums());
			m_pMapOne->Render(pd3dCommandList, pCamera);
		}
		if (pMinmapFog2) pMinmapFog2->UpdateShaderVariable(pd3dCommandList, 0);
		if (m_pMapTwo)
		{
			(m_pMapTwo)->SetItemCount(CItemMgr::GetInstance()->GetItemNums());
			m_pMapTwo->Render(pd3dCommandList, pCamera);
		}
	}
}

void CPostProcessingShader::GenerateOrthoLHMatrix(float fWidth, float fHeight, float fNearPlaneDistance, float fFarPlaneDistance)
{
	m_xmf4x4Ortho = Matrix4x4::OrthoLH(fWidth, fHeight, fNearPlaneDistance, fFarPlaneDistance);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPostProcessingByLaplacianShader::CPostProcessingByLaplacianShader()
{
}

CPostProcessingByLaplacianShader::~CPostProcessingByLaplacianShader()
{
}

D3D12_SHADER_BYTECODE CPostProcessingByLaplacianShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"HLSL_MRT.hlsl", "PSPostProcessing", "ps_5_1", ppd3dShaderBlob));
}