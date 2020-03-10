
#include "stdafx.h"
#include "Scene.h"
#include "Object_Terrain.h"

void CGameScene::PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nObjectTypeNum = 1;
	m_pObjectLists = new list<CGameObject*>*[m_nObjectTypeNum];
	
	for (int i = 0; i < m_nObjectTypeNum; i++) // ÃÊ±âÈ­
		m_pObjectLists[i] = NULL;

	m_pObjectLists[0] = new list<CGameObject*>();

	FILE* pInFile = NULL;
	::fopen_s(&pInFile, "ObjectsData/Rock.bin", "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };


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
		m_pObjectLists[0]->push_back(pGameObject);
	}

}
