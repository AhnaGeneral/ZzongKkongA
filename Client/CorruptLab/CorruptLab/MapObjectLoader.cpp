
#include "stdafx.h"
#include "Scene.h"
#include "Object_StaticObj.h"

void CGameScene::PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nObjectTypeNum = 3;
	m_pStaticObjLists = new list<CGameObject*> * [m_nObjectTypeNum];

	for (int i = 0; i < m_nObjectTypeNum; i++) // ÃÊ±âÈ­
		m_pStaticObjLists[i] = new list<CGameObject*>();


	//Rocks-------------------------------------------------------

	FILE* pInFile = NULL;
	::fopen_s(&pInFile, "ObjectsData/Rocks.bin", "rb");
	::rewind(pInFile);

	int nObjects = 0;
	(UINT)::fread(&nObjects, sizeof(int), 1, pInFile);

	CGameObject* pGameObject = NULL;
	CGameObject* pRockModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Rock.bin", NULL, false);
	for (int i = 0; i < nObjects; i++)
	{
		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread(&xmmtxWorld, sizeof(XMFLOAT4X4), 1, pInFile);
		pGameObject = new CStaticObject();
		pGameObject->SetChild(pRockModel, true);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;
		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pStaticObjLists[OBJECT_TYPE_ROCK]->push_back(pGameObject);
	}

	//Fence-----------------------------------------------

	::fopen_s(&pInFile, "ObjectsData/Fences.bin", "rb");
	::rewind(pInFile);

	nObjects = 0;
	(UINT)::fread(&nObjects, sizeof(int), 1, pInFile);

	CGameObject* pFenceModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Fence.bin", NULL, false);
	for (int i = 0; i < nObjects; i++)
	{
		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread(&xmmtxWorld, sizeof(XMFLOAT4X4), 1, pInFile);
		pGameObject = new CStaticObject();
		pGameObject->SetChild(pFenceModel, true);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;
		pGameObject->Rotate(0, -90, 0);
		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pStaticObjLists[OBJECT_TYPE_FENCE]->push_back(pGameObject);
	}


	//Tower-----------------------------------------------

	::fopen_s(&pInFile, "ObjectsData/Towers.bin", "rb");
	::rewind(pInFile);

	nObjects = 0;
	(UINT)::fread(&nObjects, sizeof(int), 1, pInFile);

	CGameObject* pTowerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/TRMTower.bin", NULL, false);
	for (int i = 0; i < nObjects; i++)
	{
		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread(&xmmtxWorld, sizeof(XMFLOAT4X4), 1, pInFile);
		pGameObject = new CStaticObject();
		pGameObject->SetChild(pTowerModel, true);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;
		pGameObject->UpdateTransform(NULL);
		pGameObject->OnInitialize();
		m_pStaticObjLists[OBJECT_TYPE_TRMTOWER]->push_back(pGameObject);
	}

}
