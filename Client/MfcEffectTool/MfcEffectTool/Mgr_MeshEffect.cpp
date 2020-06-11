#include "Mgr_MeshEffect.h"
#include "Object.h"
#include <fstream>
CMgr_EffectMesh* CMgr_EffectMesh::m_pInstance = nullptr;

void CMgr_EffectMesh::Initialize(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_ppMeshEffect = new CGameObject * [3]; 
	m_pMeshEffect = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList,
		pd3dGraphicsRootSignature, "Model/HouseAll.bin", NULL, false);
	m_pMeshEffect->SetPosition(m_EffectTransform.xmf3Default_Position);
	m_ppMeshEffect[0] = m_pMeshEffect; 

	m_pMeshEffect = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList,
		pd3dGraphicsRootSignature, "Model/Sword.bin", NULL, false);
	m_pMeshEffect->SetPosition(m_EffectTransform.xmf3Default_Position);

	m_ppMeshEffect[1] = m_pMeshEffect;
	//m_pEffectTransform = new EffectTransform; 

}

XMFLOAT3* CMgr_EffectMesh::Get_Conversion_RotationPoint()
{
	return &(m_EffectTransform.xmf3Conversion_Rotation);
}

XMFLOAT3* CMgr_EffectMesh::Get_Conversion_PositionPoint()
{
	return &(m_EffectTransform.xmf3Conversion_Position);
}

XMFLOAT3* CMgr_EffectMesh::Get_Conversion_SizePoint()
{
	return &(m_EffectTransform.xmf3Conversion_Size);
}

float* CMgr_EffectMesh::Get_Conversion_Alpha()
{
	return &(m_EffectTransform.fConversion_Alpha);
}

XMFLOAT3* CMgr_EffectMesh::Get_Default_RotationPoint()
{
	return &(m_EffectTransform.xmf3Default_Rotation);
}

XMFLOAT3* CMgr_EffectMesh::Get_Default_PositionPoint()
{
	return &(m_EffectTransform.xmf3Default_Position);
}

XMFLOAT3* CMgr_EffectMesh::Get_Default_SizePoint()
{
	return &(m_EffectTransform.xmf3Default_Size);
}

float* CMgr_EffectMesh::Get_Default_Alpha()
{
	return &(m_EffectTransform.fDefault_Alpha);
}

void CMgr_EffectMesh::Set_LifeTime(float LifeTime)
{
	m_EffectTransform.fLifeTime = LifeTime; 
}

XMFLOAT3* CMgr_EffectMesh::Get_Current_RotationPoint()
{
	return &(m_EffectTransform.xmf3Current_Rotation);
}

XMFLOAT3* CMgr_EffectMesh::Get_Current_PositionPoint()
{
	return &(m_EffectTransform.xmf3Current_Position);
}

XMFLOAT3* CMgr_EffectMesh::Get_Current_SizePoint()
{
	return &(m_EffectTransform.xmf3Current_Size);
}

float* CMgr_EffectMesh::Get_Current_Alpha()
{
	return &(m_EffectTransform.fCurrent_Alpha);
}



XMFLOAT3* CMgr_EffectMesh::Get_Speed_RotationPoint()
{
	return &(m_EffectTransform.xmf3Speed_Rotation);
}

XMFLOAT3* CMgr_EffectMesh::Get_Speed_PositionPoint()
{
	return &(m_EffectTransform.xmf3Speed_Position);
}

XMFLOAT3* CMgr_EffectMesh::Get_Speed_SizePoint()
{
	return &(m_EffectTransform.xmf3Speed_Size);
}

float* CMgr_EffectMesh::Get_Speed_Alpha()
{
	return &(m_EffectTransform.fSpeed_Alpha);
}


bool* CMgr_EffectMesh::GetBoolControl()
{
	return &m_BoolControl; 
}

void CMgr_EffectMesh::FileSave()
{
	ofstream out("HouseAll.txt");

	out << "<Conversion>:" << endl; 
	out << "<Position>:"
		<< m_EffectTransform.xmf3Conversion_Position.x
		<< m_EffectTransform.xmf3Conversion_Position.y
		<< m_EffectTransform.xmf3Conversion_Position.z << endl; 
	out << "<Rotation>:"
		<< m_EffectTransform.xmf3Conversion_Rotation.x
		<< m_EffectTransform.xmf3Conversion_Rotation.y
		<< m_EffectTransform.xmf3Conversion_Rotation.z << endl;
	out << "<Size>:"
		<< m_EffectTransform.xmf3Conversion_Size.x
		<< m_EffectTransform.xmf3Conversion_Size.y
		<< m_EffectTransform.xmf3Conversion_Size.z << endl;
	out << "<Alpha>:"
		<< m_EffectTransform.fConversion_Alpha << endl;
	out << "</Conversion>" << endl; 

	out << "<Default>:" << endl;
	out << "<Position>:"
		<< m_EffectTransform.xmf3Default_Position.x
		<< m_EffectTransform.xmf3Default_Position.y
		<< m_EffectTransform.xmf3Default_Position.z << endl;
	out << "<Rotation>:"
		<< m_EffectTransform.xmf3Default_Rotation.x
		<< m_EffectTransform.xmf3Default_Rotation.y
		<< m_EffectTransform.xmf3Default_Rotation.z << endl;
	out << "<Size>:"
		<< m_EffectTransform.xmf3Default_Size.x
		<< m_EffectTransform.xmf3Default_Size.y
		<< m_EffectTransform.xmf3Default_Size.z << endl;
	out << "<Alpha>:"
		<< m_EffectTransform.fDefault_Alpha << endl;
	out << "</Default>" << endl;

	out << "<Speed>:" << endl;
	out << "<Position>:"
		<< m_EffectTransform.xmf3Speed_Position.x
		<< m_EffectTransform.xmf3Speed_Position.y
		<< m_EffectTransform.xmf3Speed_Position.z << endl;
	out << "<Rotation>:"
		<< m_EffectTransform.xmf3Speed_Rotation.x
		<< m_EffectTransform.xmf3Speed_Rotation.y
		<< m_EffectTransform.xmf3Speed_Rotation.z << endl;
	out << "<Size>:"
		<< m_EffectTransform.xmf3Speed_Size.x
		<< m_EffectTransform.xmf3Speed_Size.y
		<< m_EffectTransform.xmf3Speed_Size.z << endl;
	out << "<Alpha>:"
		<< m_EffectTransform.fSpeed_Alpha << endl;
	out << "</Speed>" << endl;

	out << "<LifeTime>:" << m_EffectTransform.fLifeTime; 

	out.close(); 
}

void CMgr_EffectMesh::SetAlpha(float alphaValue)
{
	m_ppMeshEffect[0]->SetMeshEffectAlphaControlValue(alphaValue); 
}

void CMgr_EffectMesh::Update(float fTimeElapsed)
{

	if (m_BoolControl)
	{
		m_CountTime += 0.015f;
		// [ Position ] ================================================================================
		float DistancePosX = m_EffectTransform.xmf3Conversion_Position.x - m_EffectTransform.xmf3Current_Position.x;
		float DistancePosY = m_EffectTransform.xmf3Conversion_Position.y - m_EffectTransform.xmf3Current_Position.y;
		float DistancePosZ = m_EffectTransform.xmf3Conversion_Position.z - m_EffectTransform.xmf3Current_Position.z;

		if (DistancePosX != 0)
		{
			if (DistancePosX < 0)
			{
				m_EffectTransform.xmf3Current_Position.x += (-m_EffectTransform.xmf3Speed_Position.x);
			}
			else
			{
				m_EffectTransform.xmf3Current_Position.x += (+m_EffectTransform.xmf3Speed_Position.x);
			}
		}

		if (DistancePosY != 0)
		{
			if (DistancePosY < 0)
			{
				m_EffectTransform.xmf3Current_Position.y += (-m_EffectTransform.xmf3Speed_Position.y);
			}
			else
			{
				m_EffectTransform.xmf3Current_Position.y += (+m_EffectTransform.xmf3Speed_Position.y);
			}
		}

		if (DistancePosZ != 0)
		{
			if (DistancePosZ < 0)
			{
				m_EffectTransform.xmf3Current_Position.z += (-m_EffectTransform.xmf3Speed_Position.z);
			}
			else
			{
				m_EffectTransform.xmf3Current_Position.z += (+m_EffectTransform.xmf3Speed_Position.z);
			}
		}

		// [ Scale ] ================================================================================
		float DistanceSizeX = m_EffectTransform.xmf3Conversion_Size.x - m_EffectTransform.xmf3Current_Size.x;
		float DistanceSizeY = m_EffectTransform.xmf3Conversion_Size.y - m_EffectTransform.xmf3Current_Size.y;
		float DistanceSizeZ = m_EffectTransform.xmf3Conversion_Size.z - m_EffectTransform.xmf3Current_Size.z;

		if (DistanceSizeX != 0)
		{
			if (DistanceSizeX < 0)
			{
				m_EffectTransform.xmf3Current_Size.x += (-m_EffectTransform.xmf3Speed_Size.x);
			}
			else
			{
				m_EffectTransform.xmf3Current_Size.x += (+m_EffectTransform.xmf3Speed_Size.x);
			}
		}

		if (DistanceSizeY != 0)
		{
			if (DistanceSizeY < 0)
			{
				m_EffectTransform.xmf3Current_Size.y += (-m_EffectTransform.xmf3Speed_Size.y);
			}
			else
			{
				m_EffectTransform.xmf3Current_Size.y += (+m_EffectTransform.xmf3Speed_Size.y);
			}
		}

		if (DistanceSizeZ != 0)
		{
			if (DistanceSizeZ < 0)
			{
				m_EffectTransform.xmf3Current_Size.z += (-m_EffectTransform.xmf3Speed_Size.z);
			}
			else
			{
				m_EffectTransform.xmf3Current_Size.z += (+m_EffectTransform.xmf3Speed_Size.z);
			}
		}

		// [ Rotation ] ================================================================================
		float DistanceRoatationX = m_EffectTransform.xmf3Conversion_Rotation.x - m_EffectTransform.xmf3Current_Rotation.x;
		float DistanceRoatationY = m_EffectTransform.xmf3Conversion_Rotation.y - m_EffectTransform.xmf3Current_Rotation.y;
		float DistanceRoatationZ = m_EffectTransform.xmf3Conversion_Rotation.z - m_EffectTransform.xmf3Current_Rotation.z;

		if (DistanceRoatationX != 0)
		{
			if (DistanceRoatationX < 0)
			{
				m_EffectTransform.xmf3Current_Rotation.x += (-m_EffectTransform.xmf3Speed_Rotation.x);
			}
			else
			{
				m_EffectTransform.xmf3Current_Rotation.x += (+m_EffectTransform.xmf3Speed_Rotation.x);
			}
		}

		if (DistanceRoatationY != 0)
		{
			if (DistanceRoatationY < 0)
			{
				m_EffectTransform.xmf3Current_Rotation.y += (-m_EffectTransform.xmf3Speed_Rotation.y);
			}
			else
			{
				m_EffectTransform.xmf3Current_Rotation.y += (+m_EffectTransform.xmf3Speed_Rotation.y);
			}
		}

		if (DistanceRoatationZ != 0)
		{
			if (DistanceRoatationZ < 0)
			{
				m_EffectTransform.xmf3Current_Rotation.z += (-m_EffectTransform.xmf3Speed_Rotation.z);
			}
			else
			{
				m_EffectTransform.xmf3Current_Rotation.z += (+m_EffectTransform.xmf3Speed_Rotation.z);
			}
		}

		// [ Alpha ] ================================================================================
		float DistanceAlpha = m_EffectTransform.fConversion_Alpha - m_EffectTransform.fCurrent_Alpha;

		if (DistanceAlpha != 0)
		{
			if (DistanceAlpha < 0)
			{
				m_EffectTransform.fCurrent_Alpha += (-m_EffectTransform.fSpeed_Alpha);
			}
			else
			{
				m_EffectTransform.fCurrent_Alpha += (+m_EffectTransform.fSpeed_Alpha);
			}

			SetAlpha(m_EffectTransform.fCurrent_Alpha);
		}
		if (DistancePosX != 0 || DistancePosY != 0 || DistancePosZ != 0)
		{
			m_ppMeshEffect[0]->SetEffectPosition(m_EffectTransform.xmf3Current_Position);
		}

		if (abs(DistanceSizeX) > 0 || abs(DistanceSizeY) > 0 || abs(DistanceSizeZ) > 0)
		{
			m_ppMeshEffect[0]->SetScaleEffect(m_EffectTransform.xmf3Current_Size.x,
				m_EffectTransform.xmf3Current_Size.y, m_EffectTransform.xmf3Current_Size.z);
		}

		if (DistanceRoatationX != 0 || DistanceRoatationY != 0 || DistanceRoatationZ != 0)
		{
			m_ppMeshEffect[0]->SetEffectRotate(m_EffectTransform.xmf3Current_Rotation.x,
				m_EffectTransform.xmf3Current_Rotation.y, m_EffectTransform.xmf3Current_Rotation.z);
		}

		if (m_CountTime < m_EffectTransform.fLifeTime)
		{
			m_ppMeshEffect[0]->EffectUpdateTransform
			(m_ppMeshEffect[0]->m_xmf4x4EffectPosition, m_ppMeshEffect[0]->m_xmf4x4EffectRotation, m_ppMeshEffect[0]->m_xmf4x4EffectScale);
			//m_CountTime = 0.0f;
			//m_BoolControl = false;
		}
		else
		{
			m_CountTime = 0.0f;
			m_BoolControl = false;
		}

	}
}

void CMgr_EffectMesh::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	if (m_ppMeshEffect[0])m_ppMeshEffect[0]->Render(pd3dCommandList, pCamera);

	if (m_ppMeshEffect[1])m_ppMeshEffect[1]->Render(pd3dCommandList, pCamera);

}