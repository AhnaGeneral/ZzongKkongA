
#include "stdafx.h"
#include "Scene.h"
#include "Object_StaticObj.h"
#include "Monster_Yangmal.h"
#include "Object_ItemBox.h"
#include "Mgr_Collision.h"
#include "Object_UI.h"
#include "Shader_ObjHP.h"
#include "Shader_BillBoard.h"
//#include "Monster_Yangmal.h"

void CGameScene::PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nStaticObjectTypeNum = 3;
	m_pStaticObjLists = new list<CGameObject*> * [m_nStaticObjectTypeNum];

	for (int i = 0; i < m_nStaticObjectTypeNum; i++) // 초기화
		m_pStaticObjLists[i] = new list<CGameObject*>();

	m_nDynamicObjectTypeNum = 1;
	m_pDynamicObjLists = new list<CDynamicObject*> * [m_nDynamicObjectTypeNum];
	for (int i = 0; i < m_nDynamicObjectTypeNum; i++)
		m_pDynamicObjLists[i] = new list<CDynamicObject*> ();

	m_nMonsterTypeNum = 1;
	m_pMonsterLists = new list<CMonster*> * [m_nMonsterTypeNum];

	for (int i = 0; i < m_nStaticObjectTypeNum; i++) // 초기화
		m_pMonsterLists[i] = new list<CMonster*>();
	
	
	/*Static*/

	//Rocks-------------------------------------------------------

	CGameObject* pRockModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Rock.bin", NULL, false);
	PlaceStaticObjectsFromFile(pRockModel, "ObjectsData/Rocks.bin", OBJECT_TYPE_ROCK);

	//Fence-----------------------------------------------

	CGameObject* pFenceModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Fence.bin", NULL, false);
	PlaceStaticObjectsFromFile(pFenceModel, "ObjectsData/Fences.bin", OBJECT_TYPE_FENCE);


	//Tower-----------------------------------------------
	CGameObject* pTowerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/TRMTower.bin", NULL, false);
	PlaceStaticObjectsFromFile(pTowerModel, "ObjectsData/Towers.bin", OBJECT_TYPE_TRMTOWER);


	/*Dynamic*/
	//ItemBox--------------------------------------------
	CGameObject* pItemBox = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Box_Opening.bin", NULL, true);

	PlaceDynamicFromFile(pItemBox, "ObjectsData/ItemBoxes.bin", OBJECT_TYPE_ITEMBOX);



	/*Monster*/
	//Yangmal-------------------------------------------------
	

	CGameObject* pYangmalModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Monster/Yangmal.bin", NULL, true);
	PlaceMonsterFromFile(pYangmalModel, "ObjectsData/Yangmals.bin", MONSTER_TYPE_YANGMAL, pd3dDevice,pd3dCommandList);

	CCollisionMgr::GetInstance()->m_pMonsterLists = m_pMonsterLists;
}

void CGameScene::PlaceStaticObjectsFromFile(CGameObject* pModel, char* FileName, int index)
{

	FILE* pInFile = NULL;
	::fopen_s(&pInFile, FileName, "rb");
	::rewind(pInFile);

	int nObjects = 0;
	(UINT)::fread(&nObjects, sizeof(int), 1, pInFile);

	XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
	XMFLOAT4 xmf4Rotation;

	CGameObject* pGameObject = NULL;
	for (int i = 0; i < nObjects; i++)
	{
		(UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
		(UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
		(UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
		(UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion

		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread(&xmmtxWorld, sizeof(XMFLOAT4X4), 1, pInFile);
		pGameObject = new CStaticObject();
		pGameObject->SetChild(pModel, true);

		pGameObject->m_xmf4Rotation = xmf4Rotation;
		pGameObject->m_xmf3Scale = Vector3::ScalarProduct(xmf3Scale, 0.5f, false);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;

		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pStaticObjLists[index]->push_back(pGameObject);
	}
	fclose(pInFile);
}

void CGameScene::PlaceDynamicFromFile(CGameObject* pModel, char* FileName, int index)
{

	FILE* pInFile = NULL;
	::fopen_s(&pInFile, FileName, "rb");
	::rewind(pInFile);

	int nObjects = 0;
	(UINT)::fread(&nObjects, sizeof(int), 1, pInFile);

	XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
	XMFLOAT4 xmf4Rotation;

	CDynamicObject* pGameObject = NULL;
	for (int i = 0; i < nObjects; i++)
	{
		(UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
		(UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
		(UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
		(UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion

		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread(&xmmtxWorld, sizeof(XMFLOAT4X4), 1, pInFile);

		if (index == OBJECT_TYPE_ITEMBOX)
			pGameObject = new CItemBox();

		pGameObject->SetChild(pModel, true);

		pGameObject->m_xmf4Rotation = xmf4Rotation;
		pGameObject->m_xmf3Scale = Vector3::ScalarProduct(xmf3Scale, 0.5f, false);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;

	
		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pDynamicObjLists[index]->push_back(pGameObject);
	}
	fclose(pInFile);
}

void CGameScene::PlaceMonsterFromFile(CGameObject* pModel, char* FileName, int index, ID3D12Device* pd3dDevice,ID3D12GraphicsCommandList* pd3dCommandList)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, FileName, "rb");
	::rewind(pInFile);

	int nObjects = 0;
	(UINT)::fread(&nObjects, sizeof(int), 1, pInFile);

	XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
	XMFLOAT4 xmf4Rotation;


	CTexture* pMonsterHPTex = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pMonsterHPTex->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"UserInterface/HP/MonsterHP.dds", 0);
	CShader* pShder = new CShader_MonsterHP();
	//CShader* pShder = new CCloudGSShader();
	
	pShder->CreateShader(pd3dDevice, GetGraphicsRootSignature(), FINAL_MRT_COUNT);
	pShder->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 1);
	pShder->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pMonsterHPTex, ROOT_PARAMETER_ALBEDO_TEX, 0);

	CMonster* pGameObject = NULL;
	
	for (int i = 0; i < nObjects; i++)
	{
		(UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
		(UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
		(UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
		(UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion

		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread(&xmmtxWorld, sizeof(XMFLOAT4X4), 1, pInFile);
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
		pGameObject->m_xmf3Scale = Vector3::ScalarProduct(xmf3Scale, 0.5f, false);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;
		pGameObject->SetScale(2, 2, 2);

		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pMonsterLists[index]->push_back(pGameObject);
	}
	fclose(pInFile);
}


void CGameScene::ChangeTerrainPipeline()
{
	m_bPipelineStateIndex = (!m_bPipelineStateIndex);
}
