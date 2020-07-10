#include "Mgr_IndoorControl.h"
#include "Scene_Game2.h"
#include "Shader_LightMRT.h"

CMgr_IndoorControl* CMgr_IndoorControl::m_pInstance = NULL;

void CMgr_IndoorControl::Initialize()
{
	listpassword = new std::list<int>; 
}

void CMgr_IndoorControl::Update(float fElapsedTime)
{
}

int CMgr_IndoorControl::GetPasswordControl()
{
	return passwordControl;
}

void CMgr_IndoorControl::SetpasswordControl(int _passwordControl)
{
	passwordControl = _passwordControl; 
}

std::list<int>* CMgr_IndoorControl::GetlistPassword()
{
	return listpassword;
}

void CMgr_IndoorControl::InsertPassword(int i)
{
	if(listpassword->size() < 4)
		listpassword->push_back(i);
}

void CMgr_IndoorControl::EraserPassword()
{
	if (!listpassword->empty())
		listpassword->pop_back();
}

bool CMgr_IndoorControl::ConfirmPassword()
{
	const int ThatIsRightPassword[4] = { 8,2,7,7 };
	if (listpassword->size() != 4) return false; 
	else
	{
		int i = 0; 
		for (auto& a : *listpassword)
			if (ThatIsRightPassword[i++] != a) return false; 
		return true;
	}
}

void CMgr_IndoorControl::SetThatIsRightPassword(bool _ThatIsRight)
{
	ThatIsRightPassword = _ThatIsRight; 
	SetExecuteConfirmControl(true);
}

void CMgr_IndoorControl::SetExecuteConfirmControl(bool _Excute)
{
	ExecuteConfirmControl = _Excute; 
}

bool CMgr_IndoorControl::GetThatIsRightPassword()
{
	return ThatIsRightPassword;
}

bool CMgr_IndoorControl::GetExecuteConfirmControl()
{
	return ExecuteConfirmControl;
}

void CMgr_IndoorControl::SetIndoorSceneAndLight(CGameScene2* _IndoorScene, CLightTarget* _LightTarget)
{
	pIndoorScene = _IndoorScene;
	pIndoorLightTarget = _LightTarget;
}

void CMgr_IndoorControl::ExcuteAnimationDoor()
{
	pIndoorScene->ExcuteAnimationDoor();
	pIndoorLightTarget->TurnOnLabatoryLight();
}

void CMgr_IndoorControl::Destroy()
{
}
