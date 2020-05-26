// EffectTool.cpp: 구현 파일
//

#include "pch.h"
#include "MfcEffectTool.h"
#include "EffectTool.h"
#include "afxdialogex.h"


// EffectTool 대화 상자

IMPLEMENT_DYNAMIC(EffectTool, CDialogEx)

EffectTool::EffectTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EffectTool, pParent)
	, m_CurrentPosX(0)
	, m_CurrentPosY(0)
	, m_CurrentPosZ(0)
{

}

EffectTool::~EffectTool()
{
}

void EffectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_CurrentPosX);
	DDX_Text(pDX, IDC_EDIT25, m_CurrentPosY);
	DDX_Text(pDX, IDC_EDIT26, m_CurrentPosZ);
}


BEGIN_MESSAGE_MAP(EffectTool, CDialogEx)
	///ON_BN_CLICKED(IDC_BUTTON2, &EffectTool::ClickPosUp)
	ON_BN_CLICKED(IDC_BUTTON45, &EffectTool::OnBnClickedButton45)
END_MESSAGE_MAP()


// EffectTool 메시지 처리기




//void EffectTool::ClickPosUp()
//{
//	UpdateData(true);
//	//커서의 위치 아무것도 안 누르면 -1
//	//int index = m_ListBox.GetCurSel();
//
//	//if (index == -1) return; 
//
//	//if (index == 0) m_posX += 1; 
//	//if (index == 1) m_posY += 1;
//
//
//	UpdateData(false);
//	//그리기를 꼭 갱신을 해줘야 한다.
//}


BOOL EffectTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_ListBox.InsertString(0, L"PosX");
	//m_ListBox.InsertString(1, L"PosY");
	//m_ListBox.SetCurSel(-1);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void EffectTool::OnBnClickedButton45()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
