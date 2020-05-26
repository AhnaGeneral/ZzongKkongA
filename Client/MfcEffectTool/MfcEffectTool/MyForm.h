#pragma once
// MyForm 폼 보기
#include"EffectTool.h"
#include"EffectTool2.h"

class MyForm : public CFormView
{
	DECLARE_DYNCREATE(MyForm)

protected:
	MyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~MyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MyForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	EffectTool   * m_pEffectTool = nullptr; 
	EffectTool2  * m_pEffectTool2 = nullptr; 
	
	virtual void OnInitialUpdate();
	CTabCtrl m_TabControl; 
	afx_msg void OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult);
};


