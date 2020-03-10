
#include "stdafx.h"
#include "Scene.h"
#include "Object_Terrain.h"

void CGameScene::PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nObjectTypeNum = 2;
	m_pObjectLists = new list<CGameObject*>*[m_nObjectTypeNum];
	
	for (int i = 0; i < m_nObjectTypeNum; i++) // ÃÊ±âÈ­
		m_pObjectLists[i] = new list<CGameObject*>();
	
	
	//Rocks-------------------------------------------------------
	
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, "ObjectsData/Rocks.bin", "rb");
	::rewind(pInFile);

	int nObjects = 0;
	(UINT)::fread(&nObjects, sizeof(int), 1, pInFile);

	CGameObject* pGameObject = NULL;
	CGameObject* pRockModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Rock.bin", NULL, false);
	for (int i =0; i<nObjects ;i++ )
	{
		XMFLOAT4X4 xmmtxWorld;
		(UINT)::fread(&xmmtxWorld, sizeof(XMFLOAT4X4), 1, pInFile);
		pGameObject = new CGameObject();
		pGameObject->SetChild(pRockModel,true);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;
		pGameObject->UpdateTransform(NULL);
		m_pObjectLists[OBJECT_TYPE_ROCK]->push_back(pGameObject);
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
		pGameObject = new CGameObject();
		pGameObject->SetChild(pFenceModel, true);
		pGameObject->m_xmf4x4Transform = xmmtxWorld;
		pGameObject->UpdateTransform(NULL);
		m_pObjectLists[OBJECT_TYPE_FENCE]->push_back(pGameObject);
	}


}
