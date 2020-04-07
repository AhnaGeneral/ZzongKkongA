
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

	CGameObject* pRockModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Rock.bin", NULL, false);
	PlaceObjectsFromFile(pRockModel, "ObjectsData/Rocks.bin", OBJECT_TYPE_ROCK);

	//Fence-----------------------------------------------

	CGameObject* pFenceModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Fence.bin", NULL, false);
	PlaceObjectsFromFile(pFenceModel, "ObjectsData/Fences.bin", OBJECT_TYPE_FENCE);


	//Tower-----------------------------------------------
	CGameObject* pTowerModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/TRMTower.bin", NULL, false);
	PlaceObjectsFromFile(pTowerModel, "ObjectsData/Towers.bin", OBJECT_TYPE_TRMTOWER);
	
}

void CGameScene::PlaceObjectsFromFile(CGameObject* pModel, char* FileName, int index)
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
}
