// MyForm.cpp: 구현 파일
//

#include "pch.h"
#include "MfcEffectTool.h"
#include "MyForm.h"
#include "MfcEffectTool.h"


// MyForm

IMPLEMENT_DYNCREATE(MyForm, CFormView)

MyForm::MyForm()
	: CFormView(IDD_MyForm)
{

}

MyForm::~MyForm()
{
}

void MyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2, m_TabControl);
}

BEGIN_MESSAGE_MAP(MyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &MyForm::OnTcnSelchangeTab2)
END_MESSAGE_MAP()


// MyForm 진단

#ifdef _DEBUG
void MyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyForm 메시지 처리기


void MyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CRect rc; 
	m_TabControl.GetWindowRect(&rc); 

	m_pEffectTool = new EffectTool; 
	m_pEffectTool->Create(IDD_EffectTool, &m_TabControl); 
	m_pEffectTool->MoveWindow(0, 25, rc.Width(), rc.Height()); 
	m_pEffectTool->ShowWindow(SW_SHOW); 

	m_pEffectTool2 = new EffectTool2; 
	m_pEffectTool2->Create(IDD_EffectTool2, &m_TabControl);
	m_pEffectTool2->MoveWindow(0, 25, rc.Width(), rc.Height());
	m_pEffectTool2->ShowWindow(SW_HIDE);

	
	m_TabControl.InsertItem(0, L"EFT_Tool (00)"); 
	m_TabControl.InsertItem(1, L"EFT_Tool (01)");

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void MyForm::OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult)
{
	int uCur = m_TabControl.GetCurSel(); 

	switch (uCur)
	{
	case 0:
		m_pEffectTool->ShowWindow(SW_SHOW);
		m_pEffectTool2->ShowWindow(SW_HIDE);
		break; 

	case 1:
		m_pEffectTool->ShowWindow(SW_HIDE);
		m_pEffectTool2->ShowWindow(SW_SHOW);
		break;

	default:
		break;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
