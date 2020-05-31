// EffectTool.cpp: 구현 파일
//

#include "pch.h"
#include "MfcEffectTool.h"
#include "EffectTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "MfcEffectToolView.h"
#include "Mgr_TextureEffect.h"
#include "Mgr_MeshEffect.h"

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
	, m_FinalValue_PosX(0)
	, m_FinalValue_PosY(0)
	, m_FinalValue_PosZ(0)
	, m_FinalValue_RotX(0)
	, m_FinalValue_RotY(0)
	, m_FinalValue_RotZ(0)
	, m_FinalValue_CX(0)
	, m_FinalValue_CY(0)
	, m_FinalValue_CZ(0)
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
	DDX_Text(pDX, IDC_EDIT12, m_FinalValue_PosX);
	DDX_Text(pDX, IDC_EDIT13, m_FinalValue_PosY);
	DDX_Text(pDX, IDC_EDIT14, m_FinalValue_PosZ);
	DDX_Text(pDX, IDC_EDIT15, m_FinalValue_RotX);
	DDX_Text(pDX, IDC_EDIT16, m_FinalValue_RotY);
	DDX_Text(pDX, IDC_EDIT17, m_FinalValue_RotZ);
	DDX_Text(pDX, IDC_EDIT18, m_FinalValue_CX);
	DDX_Text(pDX, IDC_EDIT19, m_FinalValue_CY);
	DDX_Text(pDX, IDC_EDIT20, m_FinalValue_CZ);
}


BEGIN_MESSAGE_MAP(EffectTool, CDialogEx)
	ON_BN_CLICKED(IDC_FristValue_PosX_UP, &EffectTool::OnBnClickedFristvaluePosxUp)
	ON_BN_CLICKED(IDC_FristValue_PosX_Down, &EffectTool::OnBnClickedFristvaluePosxDown)
	ON_BN_CLICKED(IDC_FristValue_PosY_UP, &EffectTool::OnBnClickedFristvaluePosyUp)
	ON_BN_CLICKED(IDC_FristValue_PosY_Down, &EffectTool::OnBnClickedFristvaluePosyDown)
	ON_BN_CLICKED(IDC_FristValue_PosZ_UP, &EffectTool::OnBnClickedFristvaluePoszUp)
	ON_BN_CLICKED(IDC_FristValue_PosZ_Down, &EffectTool::OnBnClickedFristvaluePoszDown)
	ON_BN_CLICKED(IDC_FinalValue_PosX_UP, &EffectTool::OnBnClickedFinalvaluePosxUp)
	ON_BN_CLICKED(IDC_FinalValue_PosX_Down, &EffectTool::OnBnClickedFinalvaluePosxDown)
	ON_BN_CLICKED(IDC_FinalValue_PosY_UP, &EffectTool::OnBnClickedFinalvaluePosyUp)
	ON_BN_CLICKED(IDC_FinalValue_PosY_Down, &EffectTool::OnBnClickedFinalvaluePosyDown)
	ON_BN_CLICKED(IDC_FinalValue_PosZ_UP, &EffectTool::OnBnClickedFinalvaluePoszUp)
	ON_BN_CLICKED(IDC_FinalValue_PosZ_Down, &EffectTool::OnBnClickedFinalvaluePoszDown)
	ON_BN_CLICKED(IDC_FinalValue_RotX_UP, &EffectTool::OnBnClickedFinalvalueRotxUp)
	ON_BN_CLICKED(IDC_FinalValue_RotX_Down, &EffectTool::OnBnClickedFinalvalueRotxDown)
	ON_BN_CLICKED(IDC_FinalValue_RotY_UP, &EffectTool::OnBnClickedFinalvalueRotyUp)
	ON_BN_CLICKED(IDC_FinalValue_RotY_Down, &EffectTool::OnBnClickedFinalvalueRotyDown)
	ON_BN_CLICKED(IDC_FinalValue_RotZ_UP, &EffectTool::OnBnClickedFinalvalueRotzUp)
	ON_BN_CLICKED(IDC_FinalValue_RotZ_Down, &EffectTool::OnBnClickedFinalvalueRotzDown)
	ON_BN_CLICKED(IDC_FinalValue_CX_UP, &EffectTool::OnBnClickedFinalvalueCxUp)
	ON_BN_CLICKED(IDC_FinalValue_CX_Down, &EffectTool::OnBnClickedFinalvalueCxDown)
	ON_BN_CLICKED(IDC_FinalValue_CY_UP, &EffectTool::OnBnClickedFinalvalueCyUp)
	ON_BN_CLICKED(IDC_FinalValue_CY_Down, &EffectTool::OnBnClickedFinalvalueCyDown)
	ON_BN_CLICKED(IDC_FinalValue_CZ_UP, &EffectTool::OnBnClickedFinalvalueCzUp)
	ON_BN_CLICKED(IDC_FinalValue_CZ_Down, &EffectTool::OnBnClickedFinalvalueCzDown)
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
	//static_cast <CGameScene*>(pForm->gGameFramework->m_pScene[SCENE_STAGE_OUTDOOR])
	//	->moveEffectTexture(XMFLOAT3(m_FristValue_PosX,0,0));

	CMgr_EffectTex::GetInstance()->SetPosition()->x = m_FristValue_PosX;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePosxDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	m_FristValue_PosX -= 1.0f;
	CMgr_EffectTex::GetInstance()->SetPosition()->x = m_FristValue_PosX;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePosyUp()
{
	UpdateData(true);
	m_FristValue_PosY += 1.0f;
	CMgr_EffectTex::GetInstance()->SetPosition()->y = m_FristValue_PosY;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePosyDown()
{
	UpdateData(true);
	m_FristValue_PosY -= 1.0f;
	CMgr_EffectTex::GetInstance()->SetPosition()->y = m_FristValue_PosY;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePoszUp()
{
	UpdateData(true);
	m_FristValue_PosZ += 1.0f;
	CMgr_EffectTex::GetInstance()->SetPosition()->z = m_FristValue_PosZ;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePoszDown()
{
	UpdateData(true);
	m_FristValue_PosZ -= 1.0f;
	CMgr_EffectTex::GetInstance()->SetPosition()->z = m_FristValue_PosZ;
	UpdateData(false);
}

// [ 변환값 컨트롤 함수 ] ========================================================================
void EffectTool::OnBnClickedFinalvaluePosxUp()
{
	UpdateData(true);
	m_FinalValue_PosX += 1.0f; 
	CMgr_EffectMesh::GetInstance()->GetPositionPoint()->x += 1.0f; 
	CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePosxDown()
{
	UpdateData(true);
	m_FinalValue_PosX -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetPositionPoint()->x -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePosyUp()
{
	UpdateData(true);
	m_FinalValue_PosY += 1.0f;
	CMgr_EffectMesh::GetInstance()->GetPositionPoint()->y += 1.0f;
	CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePosyDown()
{
	UpdateData(true);
	m_FinalValue_PosY -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetPositionPoint()->y -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePoszUp()
{
	UpdateData(true);
	m_FinalValue_PosZ += 1.0f;
	CMgr_EffectMesh::GetInstance()->GetPositionPoint()->z += 1.0f;
	CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePoszDown()
{
	UpdateData(true);
	m_FinalValue_PosZ -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetPositionPoint()->z -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotxUp()
{
	UpdateData(true);
	m_FinalValue_RotX += 1.0f;
	CMgr_EffectMesh::GetInstance()->GetRotationPoint()->x += 1.0f;
	CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotxDown()
{
	UpdateData(true);
	m_FinalValue_RotX -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetRotationPoint()->x -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotyUp()
{
	UpdateData(true);
	m_FinalValue_RotY += 1.0f;
	CMgr_EffectMesh::GetInstance()->GetRotationPoint()->y += 1.0f;
	CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotyDown()
{
	UpdateData(true);
	m_FinalValue_RotY -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetRotationPoint()->y -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotzUp()
{
	UpdateData(true);
	m_FinalValue_RotZ += 1.0f;
	CMgr_EffectMesh::GetInstance()->GetRotationPoint()->z += 1.0f;
	CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotzDown()
{
	UpdateData(true);
	m_FinalValue_RotZ -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetRotationPoint()->z -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCxUp()
{
    UpdateData(true);
	m_FinalValue_CX += 1.0f;
	CMgr_EffectMesh::GetInstance()->GetSizePoint()->x += 1.0f;
	CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCxDown()
{
	UpdateData(true);
	m_FinalValue_CX -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetSizePoint()->x -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCyUp()
{
	UpdateData(true);
	m_FinalValue_CY += 1.0f;
	CMgr_EffectMesh::GetInstance()->GetSizePoint()->y += 1.0f;
	CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCyDown()
{
	UpdateData(true);
	m_FinalValue_CY -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetSizePoint()->y -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCzUp()
{
	UpdateData(true);
	m_FinalValue_CZ += 1.0f;
	CMgr_EffectMesh::GetInstance()->GetSizePoint()->z += 1.0f;
	CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCzDown()
{
	UpdateData(true);
	m_FinalValue_CZ -= 1.0f;
	CMgr_EffectMesh::GetInstance()->GetSizePoint()->z -= 1.0f;
	CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}
