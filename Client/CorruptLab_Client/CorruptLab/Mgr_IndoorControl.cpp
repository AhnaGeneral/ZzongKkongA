#include "Mgr_IndoorControl.h"

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

void CMgr_IndoorControl::Destroy()
{
}
