#pragma once
#include "Object_StaticObj.h"

class CUI_MonsterHP;

class CMonster : public CGameObject
{
public:
	 CMonster();
	virtual ~CMonster();

protected:
	//GameContents

	int				m_iMaxHP;
	int				m_iCurrentHP;
	int				m_iAtt ;
	bool			m_bNotice ;
	float			m_fDistanceToPlayer;
	float			m_fSpeed;
	XMFLOAT3		m_xmf3PlayerPosition;

	CCollisionBox*	m_pAttCollision;
	CCollisionBox*  m_pBodyCollision;
	CUI_MonsterHP*  m_HPUI;
	///
public:

	int				m_iState = MONSTER_STATE_IDLE;
	//GameContents
	
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, int nPipelineState = 0);
	inline void SetDistanceToPlayer(const float& distance) { m_fDistanceToPlayer = distance; }
	inline float GetDistanceToPlayer() { return m_fDistanceToPlayer; }
	inline void SetPlayerPosition(const XMFLOAT3& pos) { m_xmf3PlayerPosition = pos; }
	void SetHPUI(CUI_MonsterHP* pHP);
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL, void* pContext = NULL);
	void GetDamaage(int iDamage);
	void GetFullHP() { m_iCurrentHP = m_iMaxHP; }
	///

};

