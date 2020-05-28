// EffectTool.cpp: 구현 파일
//

#include "pch.h"
#include "MfcEffectTool.h"
#include "EffectTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "MfcEffectToolView.h"

// EffectTool 대화 상자

IMPLEMENT_DYNAMIC(EffectTool, CDialogEx)

EffectTool::EffectTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EffectTool, pParent)
	, m_CurrentPosX(0)
	, m_CurrentPosY(0)
	, m_CurrentPosZ(0)
	, m_FristValue_PosX(0)
	, m_FristValue_PosY(0)
	, m_FristValue_PosZ(0)
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
	DDX_Text(pDX, IDC_EDIT24, m_FristValue_PosX);
	DDX_Text(pDX, IDC_EDIT4, m_FristValue_PosY);
	DDX_Text(pDX, IDC_EDIT5, m_FristValue_PosZ);
}


BEGIN_MESSAGE_MAP(EffectTool, CDialogEx)
	ON_BN_CLICKED(IDC_FristValue_PosX_UP, &EffectTool::OnBnClickedFristvaluePosxUp)
	ON_BN_CLICKED(IDC_FristValue_PosX_Down, &EffectTool::OnBnClickedFristvaluePosxDown)
END_MESSAGE_MAP()


BOOL EffectTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_ListBox.InsertString(0, L"PosX");
	//m_ListBox.InsertString(1, L"PosY");
	//m_ListBox.SetCurSel(-1);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	pForm = dynamic_cast<CMfcEffectToolView*>(pMainFrame->m_mainSplite.GetPane(0, 1));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void EffectTool::OnBnClickedFristvaluePosxUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true); 
	m_FristValue_PosX += 1.0f; 
	//pForm->gGameFramework
	dynamic_cast <CGameScene*>(pForm->gGameFramework->m_pScene[SCENE_STAGE_OUTDOOR])
		->moveEffectTexture(XMFLOAT3(m_FristValue_PosX,0,0));

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePosxDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	m_FristValue_PosX -= 1.0f;
	UpdateData(false);
}
