#pragma once
#include "stdafx.h"
#include <iostream>
#include <list>

class CGameFramework;

class CMgr_IndoorControl
{
private:
	int passwordControl = 0; 
	std::list<int>* listpassword = nullptr;
	static CMgr_IndoorControl* m_pInstance;
public:
	void Initialize();
	void Update(float fElapsedTime);
	int GetPasswordControl();
	void SetpasswordControl(int _passwordControl);
	std::list<int>* GetlistPassword();

	void InsertPassword(int i);
	void EraserPassword();


	static CMgr_IndoorControl* GetInstance(void)
	{
		if (m_pInstance == NULL)
			m_pInstance = new CMgr_IndoorControl();
		return m_pInstance;
	}
	void Destroy();
};