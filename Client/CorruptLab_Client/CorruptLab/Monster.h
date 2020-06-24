#pragma once
#include "Object_StaticObj.h"
#include "Object_Nosie.h"

class CUI_MonsterHP;
class CHeightMapTerrain;


/*
������ �ൿ ����
1.�÷��̾ �˾������� ���������� �����ϰ� ���ƴٴϸ� ����
	- �������� ���ƴٴϴ� ��ġ�� �ʵ��� �߽����� ���� �̻� ����� �ʴ´�.
2. �÷��̾���� �Ÿ��� 70 �����̸� �÷��̾ ����´�.
	 - �÷��̾���� �Ÿ��� 40 �̻��̸� �پ���� �����Ѵ�.
	 - �÷��̾���� �Ÿ��� 40 �̸��̸� �ɾ�´�.
3. �÷��̾�� �ǰݴ��ϸ� DAMAGED ���°� �ȴ�.
	- DAMAGED �ִϸ��̼��� ���������� ���ݹްų� ������������ �ʴ´�.
4. ü���� 20 ���ϰ� �Ǹ� STUN ���°� �ȴ�.
	- STUN �ִϸ��̼��� �ѹ��� ����ȴ�.
5. �÷��̾���� �Ÿ��� 70 �̻��̰ų� �ʵ��� �߽������� 100 �̻� �־����� 1�� ���·� ���ư���.
	- RETURNING ���·� �����Ǹ� �̶��� �÷��̾ �ν����� �ʴ´�.
	- ������ ���߿� �ǰݵǸ� �ٽ� �÷��̾ �ν��Ѵ�.
*/

class CMonster : public CGameObject
{
public:
	 CMonster();
	virtual ~CMonster();

protected:
	//GameContents

	CB_NOISEBUFFERTYPE* m_pcbMappedNoiseBuffers = NULL;
	float			m_fDissolveTime = 0;
	ID3D12Resource* m_pd3dDissolveTime = NULL;
	float			m_fFurifyingTime = 0;


	CAnimationController* m_pAnimationController = NULL;
	int				m_iMaxHP;
	int				m_iCurrentHP;
	int				m_iAtt ;
	bool			m_bNotice ;
	float			m_fDistanceToPlayer;
	float			m_fSpeed;
	float			m_fIdleTick;
	bool			m_bIsPurified;
	XMFLOAT3		m_xmf3PlayerPosition;
	XMFLOAT3		m_xmf3RandomMoveDest;
	XMFLOAT3		m_xmf3FiledCenter;

	CCollisionBox*	m_pAttCollision;
	CCollisionBox*  m_pBodyCollision;
	CUI_MonsterHP*  m_HPUI;
	///
public:

	void Initialize(XMFLOAT3 FiledPos, int iAtt);
	bool		IsPurified() { return m_bIsPurified; }
	int				m_iState = MONSTER_STATE_IDLE;
	//GameContents
	virtual float MoveToTarget(XMFLOAT3& pos, float fTimeElapsed, float Speed, CHeightMapTerrain* pTerrain = NULL);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	inline void SetDistanceToPlayer(const float& distance) { m_fDistanceToPlayer = distance; }
	inline float GetDistanceToPlayer() { return m_fDistanceToPlayer; }
	inline void SetPlayerPosition(const XMFLOAT3& pos) { m_xmf3PlayerPosition = pos; }
	void SetHPUI(CUI_MonsterHP* pHP);
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL, void* pContext = NULL);
	virtual void BadUpdate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL, void* pContext = NULL);
	virtual void GoodUpdate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL, void* pContext = NULL);
	virtual void PurifyingUpdate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL, void* pContext = NULL);

	virtual void GetPurified();
	
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);


	virtual void OnInitialize();
	void GetDamaage(int iDamage);
	void GetFullHP() { m_iCurrentHP = m_iMaxHP; }
	///

};

