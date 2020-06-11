
#include "stdafx.h"
#include "Scene.h"
#include "Object_StaticObj.h"
#include "Monster_Yangmal.h"
#include "Object_ItemBox.h"
#include "Object_DrugMaker.h"
#include "Mgr_Collision.h"
#include "Object_UI.h"
#include "Shader_ObjHP.h"
#include "Shader_BillBoard.h"
#include "Shader_Standard.h"
//#include "Monster_Yangmal.h"

#define MAXSTATICEVEC 100 
#define MAXDYNAMICVEC 100
#define MAXMONSTERVEC 100 

void CGameScene::PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nStaticObjectTypeNum = 7;
	m_pStaticObjLists = new vector<CGameObject*>*[m_nStaticObjectTypeNum];

	for (int i = 0; i < m_nStaticObjectTypeNum; i++)
		m_pStaticObjLists[i] = new vector<CGameObject*> ();

	//------------------------------------------------------------
	m_nDynamicObjectTypeNum = 2;
	m_pDynamicObjLists = new vector<CDynamicObject*> * [m_nDynamicObjectTypeNum];

	for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		m_pDynamicObjLists[i] = new vector<CDynamicObject*> ();

	//------------------------------------------------------------
	m_nMonsterTypeNum = 1;
	m_pMonsterLists = new vector<CMonster*> * [m_nMonsterTypeNum];

	for (int i = 0; i < m_nMonsterTypeNum; i++) // ÃÊ±âÈ­
		m_pMonsterLists[i] = new vector<CMonster*>();
	
	int CurrentIndex = 0;
	CGameObject* pDiverObject = NULL;
	/*Static*/
	//Rocks-------------------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Rock.bin", NULL, 0);
	PlaceStaticObjectsFromFile(pDiverObject, "ObjectsData/Rocks.bin", OBJECT_TYPE_ROCK);

	//Fence-----------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Fence.bin", NULL, 0);
	PlaceStaticObjectsFromFile(pDiverObject, "ObjectsData/Fences.bin", OBJECT_TYPE_FENCE);


	//Tower-----------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/TRMTower.bin", NULL, 0);
	PlaceStaticObjectsFromFile(pDiverObject, "ObjectsData/Towers.bin", OBJECT_TYPE_TRMTOWER);

	//AnimalBone------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/AnimalBon.bin", NULL, 0);
	PlaceStaticObjectsFromFile(pDiverObject, "ObjectsData/AnimalBones.bin", OBJECT_TYPE_BONE);


	//House-----------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/HouseAll.bin", NULL, 0);
	PlaceStaticObjectsFromFile(pDiverObject, "ObjectsData/Houses.bin", OBJECT_TYPE_HOUSE);


	//Tree-----------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Tree1.bin", NULL, 0);
	PlaceStaticObjectsFromFile(pDiverObject, "ObjectsData/Trees.bin", OBJECT_TYPE_TREE);

	//Bridges------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Bridge.bin", NULL, 0);
	PlaceStaticObjectsFromFile(pDiverObject, "ObjectsData/Bridges.bin", OBJECT_TYPE_BRIDGE);

	/*Dynamic*/
	//ItemBox--------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Box_Opening.bin", NULL, 1);

	PlaceDynamicFromFile(pDiverObject, "ObjectsData/ItemBoxes.bin", OBJECT_TYPE_ITEMBOX);


	////DrugMakers--------------------------------------------
	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/DrugMaker.bin", NULL, 0);
	CGameObject* Alpha = pDiverObject->FindFrame("cylinderAlpha");
	CShader* alphaShader = new CTransparentedStandardShader();
	alphaShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature, FINAL_MRT_COUNT);
	Alpha->SetShader(0, alphaShader);
	PlaceDynamicFromFile(pDiverObject, "ObjectsData/DrugMakers.bin", OBJECT_TYPE_DRUGMAKER);

	/*Monster*/
	//Yangmal-------------------------------------------------

	pDiverObject = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Monster/Yangmal.bin", NULL, 2);
	PlaceMonsterFromFile(pDiverObject, "ObjectsData/Yangmals.bin", MONSTER_TYPE_YANGMAL, pd3dDevice,pd3dCommandList);

	CCollisionMgr::GetInstance()->m_pMonsterLists = m_pMonsterLists;
}

void CGameScene::PlaceStaticObjectsFromFile(CGameObject* pModel, char* FileName, UINT index)
{

	FILE* pInFile = NULL;
	::fopen_s(&pInFile, FileName, "rb");
	::rewind(pInFile);

	UINT nObjects = 0;

	(UINT)::fread_s(&nObjects,sizeof(UINT), sizeof(UINT), 1, pInFile);

	XMFLOAT3 xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f),
		xmf3Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f), 
		xmf3Scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT4 xmf4Rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	CGameObject* pGameObject = NULL;

	m_pStaticObjLists[index]->reserve(MAXSTATICEVEC);
	for (UINT i = 0; i < nObjects; i++)
	{
		(UINT)::fread_s(&xmf3Position, sizeof(XMFLOAT3), sizeof(float), 3, pInFile);
		(UINT)::fread_s(&xmf3Rotation, sizeof(XMFLOAT3), sizeof(float), 3, pInFile); //Euler Angle
		(UINT)::fread_s(&xmf3Scale, sizeof(XMFLOAT3), sizeof(float), 3, pInFile);
		(UINT)::fread_s(&xmf4Rotation, sizeof(XMFLOAT4), sizeof(float), 4, pInFile); //Quaternion

		XMFLOAT4X4 xmmtxWorld = Matrix4x4::Identity();

		(UINT)::fread_s(&xmmtxWorld, sizeof(XMFLOAT4X4), sizeof(XMFLOAT4X4), 1, pInFile);
		pGameObject = new CStaticObject();
		pGameObject->SetChild(pModel, true);

		pGameObject->m_xmf4Rotation = xmf4Rotation;
		pGameObject->m_xmf3Scale = Vector3::ScalarProduct(xmf3Scale, 0.5f, false);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;
		pGameObject->m_iTrackNumber = i;
		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pStaticObjLists[index]->emplace_back(pGameObject);
	}
	if(pInFile) fclose(pInFile);
}

void CGameScene::PlaceDynamicFromFile(CGameObject* pModel, char* FileName, int index)
{

	FILE* pInFile = NULL;
	::fopen_s(&pInFile, FileName, "rb");
	::rewind(pInFile);

	UINT nObjects = 0;
	(UINT)::fread_s(&nObjects,sizeof(UINT), sizeof(UINT), 1, pInFile);

	XMFLOAT3 xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f),
		xmf3Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f),
		xmf3Scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT4 xmf4Rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	CDynamicObject* pGameObject = NULL;
	
	m_pDynamicObjLists[index]->reserve(MAXDYNAMICVEC);
	for (UINT i = 0; i < nObjects; i++)
	{
		(UINT)::fread_s(&xmf3Position, sizeof(XMFLOAT3), sizeof(float), 3, pInFile);
		(UINT)::fread_s(&xmf3Rotation, sizeof(XMFLOAT3), sizeof(float), 3, pInFile); //Euler Angle
		(UINT)::fread_s(&xmf3Scale, sizeof(XMFLOAT3), sizeof(float), 3, pInFile);
		(UINT)::fread_s(&xmf4Rotation, sizeof(XMFLOAT4), sizeof(float), 4, pInFile); //Quaternion

		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread_s(&xmmtxWorld, sizeof(XMFLOAT4X4), sizeof(XMFLOAT4X4), 1, pInFile);

		switch (index)
		{
		case OBJECT_TYPE_ITEMBOX:
			pGameObject = new CItemBox();
			break;
		case OBJECT_TYPE_DRUGMAKER:
			pGameObject = new CDrugMaker(i);
			break;
		default:
			pGameObject = new CDynamicObject();
			break;
		}

		pGameObject->m_iTrackNumber = i;
		pGameObject->SetChild(pModel, true);

		pGameObject->m_xmf4Rotation = xmf4Rotation;
		pGameObject->m_xmf3Scale = Vector3::ScalarProduct(xmf3Scale, 0.5f, false);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;

	
		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pDynamicObjLists[index]->emplace_back(pGameObject);
	}
	if (pInFile) fclose(pInFile);
}

void CGameScene::PlaceMonsterFromFile(CGameObject* pModel, char* FileName, int index,
	ID3D12Device* pd3dDevice,ID3D12GraphicsCommandList* pd3dCommandList)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, FileName, "rb");
	::rewind(pInFile);

	UINT nObjects = 0;
	(UINT)::fread_s(&nObjects, sizeof(UINT), sizeof(UINT), 1, pInFile);

	XMFLOAT3 xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f),
		xmf3Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f),
		xmf3Scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT4 xmf4Rotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);


	CTexture* pMonsterHPTex = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pMonsterHPTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/HP/MonsterHP.dds", 0);
	CShader* pShder = new CShader_MonsterHP();
	
	pShder->CreateShader(pd3dDevice, GetGraphicsRootSignature(), FINAL_MRT_COUNT);
	pShder->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 1);
	pShder->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pMonsterHPTex, ROOT_PARAMETER_ALBEDO_TEX, 0);

	CMonster* pGameObject = NULL;
	m_pMonsterLists[index]->reserve(MAXMONSTERVEC);

	for (UINT i = 0; i < nObjects; i++)
	{
		(UINT)::fread_s(&xmf3Position, sizeof(XMFLOAT3), sizeof(float), 3, pInFile);
		(UINT)::fread_s(&xmf3Rotation, sizeof(XMFLOAT3), sizeof(float), 3, pInFile); //Euler Angle
		(UINT)::fread_s(&xmf3Scale, sizeof(XMFLOAT3), sizeof(float), 3, pInFile);
		(UINT)::fread_s(&xmf4Rotation,sizeof(XMFLOAT4), sizeof(float), 4, pInFile); //Quaternion

		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread_s(&xmmtxWorld, sizeof(XMFLOAT4X4), sizeof(XMFLOAT4X4), 1, pInFile);
		pGameObject = new CYangmal();

		CUI_MonsterHP* hp = new CUI_MonsterHP();
	
		hp->SetShader(pShder);
		hp->SetTexture(pMonsterHPTex);

		pGameObject->SetChild(pModel, true);
		hp->CreateShaderVariables(pd3dDevice, pd3dCommandList);
		pGameObject->SetHPUI(hp);

		float scale = xmf3Scale.x / 15.f;
		XMFLOAT2 SCALE = XMFLOAT2(scale, scale / 10);
		hp->SetInstanceInfo(SCALE, pd3dDevice, pd3dCommandList);

		pGameObject->m_xmf4Rotation = xmf4Rotation;
		pGameObject->m_xmf3Scale =  Vector3::ScalarProduct(xmf3Scale, 1.8f, false);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;
		pGameObject->SetScale(2, 2, 2);
		pGameObject->m_iTrackNumber = i;

		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pMonsterLists[index]->emplace_back(pGameObject);
		//m_pMonsterLists[index]->
	}
	//pModel->m_pAnimationController = NULL;
	if (pInFile) fclose(pInFile);
}


void CGameScene::ChangeTerrainPipeline()
{
	m_bPipelineStateIndex = (!m_bPipelineStateIndex);
}
