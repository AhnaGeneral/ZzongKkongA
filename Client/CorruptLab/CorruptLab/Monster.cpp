#include "Monster.h"
#include "Mgr_Collision.h"
#include "Object_UI.h"
#include "Object_Terrain.h"

CMonster::CMonster()
{
	m_iMaxHP = 100;
	m_iCurrentHP = 80;
	m_iAtt = 5;
	m_bNotice = false;
	m_fDistanceToPlayer = 0;
	m_fSpeed = 10;
}

CMonster::~CMonster()
{
	m_HPUI->ReleaseShaderVariables();
	m_HPUI->Release();
}


void CMonster::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nPipelineState)
{
	CGameObject::Render(pd3dCommandList, pCamera, nPipelineState);
	m_HPUI->UpdateTransform(&m_xmf4x4World);
	m_HPUI->Render(pd3dCommandList, pCamera);

}

void CMonster::SetHPUI(CUI_MonsterHP* pHP)
{
	m_HPUI = pHP;
	m_HPUI->m_pcbMonsterHP = &m_iCurrentHP;

}

void CMonster::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent, void* pContext)
{
	if (m_iState == MONSTER_STATE_STUN) return;

	CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)pContext;
	if (!m_bNotice)
	{
		SetAnimationSet(0); // idle
		if (m_fDistanceToPlayer < 70)
		{
			m_bNotice = true;
			m_iState = MONSTER_STATE_WALK;
		}
	}
	else
	{
		XMFLOAT3 xmf3Position = GetPosition();

		XMFLOAT3 xmf3Look = GetLook();
		XMFLOAT3 xmf3ToTarget = Vector3::Subtract(m_xmf3PlayerPosition, xmf3Position, true);
		float fDotProduct = Vector3::DotProduct(xmf3Look, xmf3ToTarget);
		float fAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : ((fDotProduct > 0.0f) ? XMConvertToDegrees(acos(fDotProduct)) : 90.0f);
		XMFLOAT3 xmf3CrossProduct = Vector3::CrossProduct(xmf3Look, xmf3ToTarget);
		Rotate(0.0f, fAngle * fTimeElapsed * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), 0.0f);
		XMFLOAT3 MovePos = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Look, m_fSpeed * fTimeElapsed));
		
		MovePos.y = pTerrain->GetHeight(MovePos.x, MovePos.z);

		SetPosition(MovePos);

		SetAnimationSet(1); // walk
		if (m_fDistanceToPlayer > 50)
		{
			m_iState = MONSTER_STATE_IDLE;
			m_bNotice = false;
		}
		XMFLOAT4 Rotation = GetRotateQuaternion(m_xmf3Scale.x, m_pAttCollision->m_pParent->m_xmf4x4World);
		//XMMATRIX RotateMat = XMLoadFloat4x4(&m_xmf4x4World) * XMLoadFloat4x4(&inverse);
		//XMQuaternionRotationMatrix(RotateMat)
		UpdateCollisionBoxes(pxmf4x4Parent, &Rotation, &m_xmf3Scale);
		m_pAttCollision->Update(pxmf4x4Parent, &Rotation, & m_xmf3Scale);
		CCollisionMgr::GetInstance()->MonsterAttackCheck(m_iAtt, *m_pAttCollision, fTimeElapsed);
	}

	Animate(fTimeElapsed, pxmf4x4Parent);

}

void CMonster::GetDamaage(int iDamage)
{
	m_iCurrentHP -= iDamage;
	if (m_iCurrentHP <= 20) 
	{
		m_iState = MONSTER_STATE_STUN;
		m_iCurrentHP = 20;
	}
}
