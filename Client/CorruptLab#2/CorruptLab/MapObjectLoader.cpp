
#include "stdafx.h"
#include "Scene.h"
#include "Geometry.h"

void CGameScene::PlaceObjectsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_nObjectTypeNum = 1;
	m_pObjectLists = new list<CGameObject*>*[m_nObjectTypeNum];
	
	for (int i = 0; i < m_nObjectTypeNum; i++) // ÃÊ±âÈ­
		m_pObjectLists[i] = NULL;

	//CGameObject* pTempBoxMesh = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, GetGraphicsRootSignature(), "Model/Johnson/Johnson.bin", NULL, false);
	CGameObject* pBoxObj = new CGameObject();
	pBoxObj->SetPosition(0, 3, 0);
	//pBoxObj->SetChild(pTempBoxMesh, true);

	m_pObjectLists[0] = new list<CGameObject*>();
	m_pObjectLists[0]->push_back(pBoxObj);
}
