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

#define FristPositionDelta 0.1f
#define FristRotationDelta 0.1f
#define FristSizeDelta 0.1f
#define FristAlphaDelta 0.1f

#define FinalPositionDelta 10.0f
#define FinalRotationDelta 10.0f
#define FinalSizeDelta 0.1f
#define FinalAlphaDelta 0.05f

#define PosRotSpeed 0.05f
#define AlphaSpeed 0.005f

#define MeshEffect 
#define TextureEffect





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
	, m_FirstValue_RotX(0)
	, m_FirstValue_RotY(0)
	, m_FirstValue_RotZ(0)
	, m_FirstValue_CX(0)
	, m_FirstValue_CY(0)
	, m_FirstValue_CZ(0)
	, m_AlphaValue(0)
	, m_FinalValue_alpha(0)
	, m_SpeedPosX(0)
	, m_SpeedPosY(0)
	, m_SpeedPosZ(0)
	, m_SpeedRotX(0)
	, m_SpeedRotY(0)
	, m_SpeedRotZ(0)
	, m_SpeedSizeX(0)
	, m_SpeedSizeY(0)
	, m_SpeedSizeZ(0)
	, m_SpeedAlpha(0)
	, m_UpdateTime(0)
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
	DDX_Text(pDX, IDC_EDIT6, m_FirstValue_RotX);
	DDX_Text(pDX, IDC_EDIT7, m_FirstValue_RotY);
	DDX_Text(pDX, IDC_EDIT8, m_FirstValue_RotZ);
	DDX_Text(pDX, IDC_EDIT9, m_FirstValue_CX);
	DDX_Text(pDX, IDC_EDIT10, m_FirstValue_CY);
	DDX_Text(pDX, IDC_EDIT11, m_FirstValue_CZ);
	DDX_Control(pDX, IDC_LIST4, m_ListBoxTexture);
	DDX_Control(pDX, IDC_LIST3, m_ListBoxMesh);
	DDX_Text(pDX, IDC_EDIT27, m_AlphaValue);
	DDX_Text(pDX, IDC_EDIT28, m_FinalValue_alpha);
	DDX_Text(pDX, IDC_EDIT22, m_SpeedPosX);
	DDX_Text(pDX, IDC_EDIT29, m_SpeedPosY);
	DDX_Text(pDX, IDC_EDIT30, m_SpeedPosZ);
	DDX_Text(pDX, IDC_EDIT31, m_SpeedRotX);
	DDX_Text(pDX, IDC_EDIT32, m_SpeedRotY);
	DDX_Text(pDX, IDC_EDIT33, m_SpeedRotZ);
	DDX_Text(pDX, IDC_EDIT34, m_SpeedSizeX);
	DDX_Text(pDX, IDC_EDIT35, m_SpeedSizeY);
	DDX_Text(pDX, IDC_EDIT36, m_SpeedSizeZ);
	DDX_Text(pDX, IDC_EDIT37, m_SpeedAlpha);
	DDX_Text(pDX, IDC_EDIT43, m_UpdateTime);
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
	ON_BN_CLICKED(IDC_FristValue_RotX_UP, &EffectTool::OnBnClickedFristvalueRotxUp)
	ON_BN_CLICKED(IDC_FristValue_RotX_Down, &EffectTool::OnBnClickedFristvalueRotxDown)
	ON_BN_CLICKED(IDC_FristValue_RotY_UP, &EffectTool::OnBnClickedFristvalueRotyUp)
	ON_BN_CLICKED(IDC_FristValue_RotY_Down, &EffectTool::OnBnClickedFristvalueRotyDown)
	ON_BN_CLICKED(IDC_FristValue_RotZ_UP, &EffectTool::OnBnClickedFristvalueRotzUp)
	ON_BN_CLICKED(IDC_FristValue_RotZ_Down, &EffectTool::OnBnClickedFristvalueRotzDown)
	ON_BN_CLICKED(IDC_FristValue_CX_UP, &EffectTool::OnBnClickedFristvalueCxUp)
	ON_BN_CLICKED(IDC_FristValue_CX_Down, &EffectTool::OnBnClickedFristvalueCxDown)
	ON_BN_CLICKED(IDC_FristValue_CY_UP, &EffectTool::OnBnClickedFristvalueCyUp)
	ON_BN_CLICKED(IDC_FristValue_CY_Down, &EffectTool::OnBnClickedFristvalueCyDown)
	ON_BN_CLICKED(IDC_FristValue_CZ_Up, &EffectTool::OnBnClickedFristvalueCzUp)
	ON_BN_CLICKED(IDC_FristValue_CZ_Downs, &EffectTool::OnBnClickedFristvalueCzDowns)
	ON_BN_CLICKED(IDC_TextureCreate, &EffectTool::OnBnClickedTexturecreate)
	ON_BN_CLICKED(IDC_MeshCreate, &EffectTool::OnBnClickedMeshcreate)
	ON_BN_CLICKED(IDC_AnimationPlay, &EffectTool::OnBnClickedAnimationplay)
	ON_BN_CLICKED(IDC_AnimationStop, &EffectTool::OnBnClickedAnimationstop)
	ON_BN_CLICKED(IDC_AlphaValue_UP, &EffectTool::OnBnClickedAlphavalueUp)
	ON_BN_CLICKED(IDC_AlphaValue_Down, &EffectTool::OnBnClickedAlphavalueDown)
	ON_BN_CLICKED(IDC_AlphaValue_UP2, &EffectTool::OnBnClickedAlphavalueUp2)
	ON_BN_CLICKED(IDC_AlphaValue_Down2, &EffectTool::OnBnClickedAlphavalueDown2)
	ON_BN_CLICKED(IDC_BUTTON40, &EffectTool::OnBnClickedButton40)
	ON_BN_CLICKED(IDC_Speed_PosX_UP, &EffectTool::OnBnClickedSpeedPosxUp)
	ON_BN_CLICKED(IDC_Speed_PosX_Down, &EffectTool::OnBnClickedSpeedPosxDown)
	ON_BN_CLICKED(IDC_Speed_PosY_UP, &EffectTool::OnBnClickedSpeedPosyUp)
	ON_BN_CLICKED(IDC_Speed_PosY_Down, &EffectTool::OnBnClickedSpeedPosyDown)
	ON_BN_CLICKED(IDC_Speed_PosZ_UP, &EffectTool::OnBnClickedSpeedPoszUp)
	ON_BN_CLICKED(IDC_Speed_RotX_UP, &EffectTool::OnBnClickedSpeedRotxUp)
	ON_BN_CLICKED(IDC_Speed_PosZ_Down, &EffectTool::OnBnClickedSpeedPoszDown)
	ON_BN_CLICKED(IDC_Speed_RotX_Down, &EffectTool::OnBnClickedSpeedRotxDown)
	ON_BN_CLICKED(IDC_Speed_RotY_UP, &EffectTool::OnBnClickedSpeedRotyUp)
	ON_BN_CLICKED(IDC_Speed_RotY_Down, &EffectTool::OnBnClickedSpeedRotyDown)
	ON_BN_CLICKED(IDC_Speed_RotZ_UP, &EffectTool::OnBnClickedSpeedRotzUp)
	ON_BN_CLICKED(IDC_Speed_RotZ_Down, &EffectTool::OnBnClickedSpeedRotzDown)
	ON_BN_CLICKED(IDC_Speed_SizeX_UP, &EffectTool::OnBnClickedSpeedSizexUp)
	ON_BN_CLICKED(IDC_Speed_SizeX_Down, &EffectTool::OnBnClickedSpeedSizexDown)
	ON_BN_CLICKED(IDC_Speed_SizeY_UP, &EffectTool::OnBnClickedSpeedSizeyUp)
	ON_BN_CLICKED(IDC_Speed_SizeY_Down, &EffectTool::OnBnClickedSpeedSizeyDown)
	ON_BN_CLICKED(IDC_Speed_SizeZ_UP, &EffectTool::OnBnClickedSpeedSizezUp)
	ON_BN_CLICKED(IDC_Speed_SizeZ_Down, &EffectTool::OnBnClickedSpeedSizezDown)
	ON_BN_CLICKED(IDC_Speed_Alpha_UP, &EffectTool::OnBnClickedSpeedAlphaUp)
	ON_BN_CLICKED(IDC_Speed_Alpha_Down, &EffectTool::OnBnClickedSpeedAlphaDown)
END_MESSAGE_MAP()


BOOL EffectTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(true);
	m_FirstValue_CX = 1.0f;
	m_FirstValue_CZ = 1.0f;
	m_AlphaValue = 1.0f; 
	m_FinalValue_alpha = 1.0f; 
	m_FinalValue_CX = 1.0f; 
	m_FinalValue_CY = 1.0f; 
	m_FinalValue_CZ = 1.0f;

	 m_SpeedPosX = 0.1f;
	 m_SpeedPosY = 0.1f;
	 m_SpeedPosZ = 0.1f;
	 m_SpeedRotX = 0.1f;
	 m_SpeedRotY = 0.1f;
	 m_SpeedRotZ = 0.1f;
	 m_SpeedSizeX= 0.05f;
	 m_SpeedSizeY= 0.05f;
	 m_SpeedSizeZ= 0.05f;
	 m_SpeedAlpha = 0.001f;
	 m_UpdateTime = 0.0f; 


	UpdateData(false);
	//m_ListBox.InsertString(0, L"PosX");
	//m_ListBox.InsertString(1, L"PosY");
	//m_ListBox.SetCurSel(-1);
	m_ListBoxTexture.InsertString(0, L"TestTexture"); 

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	pForm = dynamic_cast<CMfcEffectToolView*>(pMainFrame->m_mainSplite.GetPane(0, 1));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

	//pForm->gGameFramework
	//static_cast <CGameScene*>(pForm->gGameFramework->m_pScene[SCENE_STAGE_OUTDOOR])
	//	->moveEffectTexture(XMFLOAT3(m_FristValue_PosX,0,0));

// [ 초기값 컨트롤 함수 ] ========================================================================
void EffectTool::OnBnClickedFristvaluePosxUp()
{
	UpdateData(true); 

	m_FristValue_PosX += FristPositionDelta;

#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_PositionPoint()->x = m_FristValue_PosX; 
	CMgr_EffectMesh::GetInstance()->Get_Current_PositionPoint()->x = m_FristValue_PosX;

#endif 

#ifdef TextureEffect
	CMgr_EffectTex::GetInstance()->SetPosition()->x = m_FristValue_PosX;
#endif 


	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePosxDown()
{
	UpdateData(true);

	m_FristValue_PosX -= FristPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_PositionPoint()->x = m_FristValue_PosX;
	CMgr_EffectMesh::GetInstance()->Get_Current_PositionPoint()->x = m_FristValue_PosX;

#endif 

	CMgr_EffectTex::GetInstance()->SetPosition()->x = m_FristValue_PosX;

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePosyUp()
{
	UpdateData(true);
	m_FristValue_PosY += FristPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_PositionPoint()->y = m_FristValue_PosY;
	CMgr_EffectMesh::GetInstance()->Get_Current_PositionPoint()->y = m_FristValue_PosY;

#endif

	CMgr_EffectTex::GetInstance()->SetPosition()->y = m_FristValue_PosY;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePosyDown()
{
	UpdateData(true);
	m_FristValue_PosY -= FristPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_PositionPoint()->y = m_FristValue_PosY;
	CMgr_EffectMesh::GetInstance()->Get_Current_PositionPoint()->y = m_FristValue_PosY;

#endif 

	CMgr_EffectTex::GetInstance()->SetPosition()->y = m_FristValue_PosY;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePoszUp()
{
	UpdateData(true);
	m_FristValue_PosZ += FristPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_PositionPoint()->z = m_FristValue_PosZ;
	CMgr_EffectMesh::GetInstance()->Get_Current_PositionPoint()->z = m_FristValue_PosY;

#endif

	CMgr_EffectTex::GetInstance()->SetPosition()->z = m_FristValue_PosZ;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvaluePoszDown()
{
	UpdateData(true);
	m_FristValue_PosZ -= FristPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_PositionPoint()->z = m_FristValue_PosZ;
	CMgr_EffectMesh::GetInstance()->Get_Current_PositionPoint()->z = m_FristValue_PosY;

#endif 

	CMgr_EffectTex::GetInstance()->SetPosition()->z = m_FristValue_PosZ;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueRotxUp()
{
	UpdateData(true);
	m_FirstValue_RotX += FristRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_RotationPoint()->x = m_FirstValue_RotX;
	CMgr_EffectMesh::GetInstance()->Get_Current_RotationPoint()->x = m_FirstValue_RotX;

#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueRotxDown()
{
	UpdateData(true);
	m_FirstValue_RotX -= FristRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_RotationPoint()->x = m_FirstValue_RotX;
	CMgr_EffectMesh::GetInstance()->Get_Current_RotationPoint()->x = m_FirstValue_RotX;

#endif

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueRotyUp()
{
	UpdateData(true);
	m_FirstValue_RotY += FristRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_RotationPoint()->y = m_FirstValue_RotY;
	CMgr_EffectMesh::GetInstance()->Get_Current_RotationPoint()->y = m_FirstValue_RotY;

#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueRotyDown()
{
	UpdateData(true);
	m_FirstValue_RotY -= FristRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_RotationPoint()->y = m_FirstValue_RotY;
	CMgr_EffectMesh::GetInstance()->Get_Current_RotationPoint()->y = m_FirstValue_RotY;

#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueRotzUp()
{
	UpdateData(true);
	m_FirstValue_RotZ += FristRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_RotationPoint()->z = m_FirstValue_RotZ;
	CMgr_EffectMesh::GetInstance()->Get_Current_RotationPoint()->z = m_FirstValue_RotZ;

#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueRotzDown()
{
	UpdateData(true);
	m_FirstValue_RotZ -= FristRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_RotationPoint()->z = m_FirstValue_RotZ;
	CMgr_EffectMesh::GetInstance()->Get_Current_RotationPoint()->z = m_FirstValue_RotZ;

#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueCxUp()
{
	UpdateData(true);
	m_FirstValue_CX += FristSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_SizePoint()->x = m_FirstValue_CX;
	CMgr_EffectMesh::GetInstance()->Get_Current_SizePoint()->x = m_FirstValue_CX;

#endif 

	*(CMgr_EffectTex::GetInstance()->SetSizeX())+= m_FirstValue_CX;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueCxDown()
{
	UpdateData(true);
	m_FirstValue_CX -= FristSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_SizePoint()->x = m_FirstValue_CX;
	CMgr_EffectMesh::GetInstance()->Get_Current_SizePoint()->x = m_FirstValue_CX;

#endif 

	*(CMgr_EffectTex::GetInstance()->SetSizeX()) -= m_FirstValue_CX;
	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueCyUp()
{
	UpdateData(true);
	m_FirstValue_CY -= FristSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_SizePoint()->y = m_FirstValue_CY;
	CMgr_EffectMesh::GetInstance()->Get_Current_SizePoint()->y = m_FirstValue_CY;

#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueCyDown()
{
	UpdateData(true);
	m_FirstValue_CY -= FristSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_SizePoint()->y = m_FirstValue_CY;
	CMgr_EffectMesh::GetInstance()->Get_Current_SizePoint()->y = m_FirstValue_CY;

#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedFristvalueCzUp()
{
	UpdateData(true);
	m_FirstValue_CZ += FristSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_SizePoint()->z = m_FirstValue_CY;
	CMgr_EffectMesh::GetInstance()->Get_Current_SizePoint()->z = m_FirstValue_CZ;

#endif 


	*(CMgr_EffectTex::GetInstance()->SetSizeZ())+= m_FirstValue_CZ;
	UpdateData(false);
}

void EffectTool::OnBnClickedFristvalueCzDowns()
{
	UpdateData(true);

	m_FirstValue_CZ -= FristSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Default_SizePoint()->z = m_FirstValue_CZ;
	CMgr_EffectMesh::GetInstance()->Get_Current_SizePoint()->z = m_FirstValue_CZ;

#endif


	*(CMgr_EffectTex::GetInstance()->SetSizeZ())-= m_FirstValue_CZ;
	UpdateData(false);
}



//=============================================================================================
// [ 변환값 컨트롤 함수 ] ========================================================================
//=============================================================================================


void EffectTool::OnBnClickedFinalvaluePosxUp()
{
	UpdateData(true);
	m_FinalValue_PosX += FinalPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_PositionPoint()->x = m_FinalValue_PosX;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePosxDown()
{
	UpdateData(true);
	m_FinalValue_PosX -= FinalPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_PositionPoint()->x = m_FinalValue_PosX;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePosyUp()
{
	UpdateData(true);
	m_FinalValue_PosY += FinalPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_PositionPoint()->y = m_FinalValue_PosY;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePosyDown()
{
	UpdateData(true);
	m_FinalValue_PosY -= FinalPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_PositionPoint()->y = m_FinalValue_PosY;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePoszUp()
{
	UpdateData(true);
	m_FinalValue_PosZ += FinalPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_PositionPoint()->z = m_FinalValue_PosZ;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvaluePoszDown()
{
	UpdateData(true);
	m_FinalValue_PosZ -= FinalPositionDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_PositionPoint()->z = m_FinalValue_PosZ;
#endif

	//CMgr_EffectMesh::GetInstance()->SetPosition();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotxUp()
{
	UpdateData(true);
	m_FinalValue_RotX += FinalRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_RotationPoint()->x = m_FinalValue_RotX;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotxDown()
{
	UpdateData(true);
	m_FinalValue_RotX -= FinalRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_RotationPoint()->x = m_FinalValue_RotX;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotyUp()
{
	UpdateData(true);
	m_FinalValue_RotY += FinalRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_RotationPoint()->y = m_FinalValue_RotY;
#endif

	//CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotyDown()
{
	UpdateData(true);
	m_FinalValue_RotY -= FinalRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_RotationPoint()->y = m_FinalValue_RotY;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotzUp()
{
	UpdateData(true);
	m_FinalValue_RotZ += FinalRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_RotationPoint()->z = m_FinalValue_RotZ;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueRotzDown()
{
	UpdateData(true);
	m_FinalValue_RotZ -= FinalRotationDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_RotationPoint()->z = m_FinalValue_RotZ;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetRotation();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCxUp()
{
    UpdateData(true);
	m_FinalValue_CX += FinalSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_SizePoint()->x = m_FinalValue_CX;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCxDown()
{
	UpdateData(true);
	m_FinalValue_CX -= FinalSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_SizePoint()->x = m_FinalValue_CX;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCyUp()
{
	UpdateData(true);
	m_FinalValue_CY += FinalSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_SizePoint()->y = m_FinalValue_CY;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCyDown()
{
	UpdateData(true);
	m_FinalValue_CY -= FinalSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_SizePoint()->y = m_FinalValue_CY;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCzUp()
{
	UpdateData(true);
	m_FinalValue_CZ += FinalSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_SizePoint()->z = m_FinalValue_CZ;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}


void EffectTool::OnBnClickedFinalvalueCzDown()
{
	UpdateData(true);
	m_FinalValue_CZ -= FinalSizeDelta;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Conversion_SizePoint()->z = m_FinalValue_CZ;
#endif 

	//CMgr_EffectMesh::GetInstance()->SetSize();
	UpdateData(false);
}



void EffectTool::OnBnClickedTexturecreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EffectTool::OnBnClickedMeshcreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EffectTool::OnBnClickedAnimationplay()
{
#ifdef MeshEffect
	if(CMgr_EffectMesh::GetInstance()->GetBoolControl())
	*(CMgr_EffectMesh::GetInstance()->GetBoolControl()) = true;
	//UpdateData(true);
	//m_UpdateTime = CMgr_EffectMesh::GetInstance()->GetCount();
	//UpdateData(false);
#endif 

}


void EffectTool::OnBnClickedAnimationstop()
{
#ifdef MeshEffect
	if (CMgr_EffectMesh::GetInstance()->GetBoolControl())
	*(CMgr_EffectMesh::GetInstance()->GetBoolControl()) = false;
#endif 

}


void EffectTool::OnBnClickedAlphavalueUp()
{
	UpdateData(true); 
	m_AlphaValue += FristAlphaDelta;
#ifdef MeshEffect
	*(CMgr_EffectMesh::GetInstance()->Get_Default_Alpha()) = m_AlphaValue; 
	*(CMgr_EffectMesh::GetInstance()->Get_Current_Alpha()) = m_AlphaValue;

#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedAlphavalueDown()
{
	UpdateData(true);
	m_AlphaValue -= FristAlphaDelta;
#ifdef MeshEffect
	*(CMgr_EffectMesh::GetInstance()->Get_Default_Alpha()) = m_AlphaValue;
	*(CMgr_EffectMesh::GetInstance()->Get_Current_Alpha()) = m_AlphaValue;

#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedAlphavalueUp2()
{
	UpdateData(true);
	m_FinalValue_alpha += FinalAlphaDelta;
#ifdef MeshEffect
	*(CMgr_EffectMesh::GetInstance()->Get_Conversion_Alpha()) = m_FinalValue_alpha;
#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedAlphavalueDown2()
{
	UpdateData(true);
	m_FinalValue_alpha -= FinalAlphaDelta;
#ifdef MeshEffect
	*(CMgr_EffectMesh::GetInstance()->Get_Conversion_Alpha()) = m_FinalValue_alpha;
#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedButton40()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// [Speed] =========================================================================================
void EffectTool::OnBnClickedSpeedPosxUp()
{
	UpdateData(true);
	m_SpeedPosX += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_PositionPoint()->x = m_SpeedPosX;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedPosxDown()
{
	UpdateData(true);
	m_SpeedPosX -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_PositionPoint()->x = m_SpeedPosX;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedPosyUp()
{
	UpdateData(true);
	m_SpeedPosY += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_PositionPoint()->y = m_SpeedPosY;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedPosyDown()
{
	UpdateData(true);
	m_SpeedPosY -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_PositionPoint()->y = m_SpeedPosY;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedPoszUp()
{
	UpdateData(true);
	m_SpeedPosZ += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_PositionPoint()->z = m_SpeedPosZ;
#endif 
	UpdateData(false);
}

void EffectTool::OnBnClickedSpeedPoszDown()
{
	UpdateData(true);
	m_SpeedPosZ -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_PositionPoint()->z = m_SpeedPosZ;
#endif 
	UpdateData(false);
}

void EffectTool::OnBnClickedSpeedRotxUp()
{
	UpdateData(true);
	m_SpeedRotX += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_RotationPoint()->x = m_SpeedRotX;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedRotxDown()
{
	UpdateData(true);
	m_SpeedRotX -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_RotationPoint()->x = m_SpeedRotX;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedRotyUp()
{
	UpdateData(true);
	m_SpeedRotY += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_RotationPoint()->y = m_SpeedRotY;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedRotyDown()
{
	UpdateData(true);
	m_SpeedRotY -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_RotationPoint()->y= m_SpeedRotY;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedRotzUp()
{
	UpdateData(true);
	m_SpeedRotZ += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_RotationPoint()->z = m_SpeedRotZ;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedRotzDown()
{
	UpdateData(true);
	m_SpeedRotZ -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_RotationPoint()->z = m_SpeedRotZ;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedSizexUp()
{
	UpdateData(true);
	m_SpeedSizeX += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_SizePoint()->x = m_SpeedSizeX;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedSizexDown()
{
	UpdateData(true);
	m_SpeedSizeX -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_SizePoint()->x = m_SpeedSizeX;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedSizeyUp()
{
	UpdateData(true);
	m_SpeedSizeY += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_SizePoint()->y = m_SpeedSizeY;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedSizeyDown()
{
	UpdateData(true);
	m_SpeedSizeY -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_SizePoint()->y = m_SpeedSizeY;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedSizezUp()
{
	UpdateData(true);
	m_SpeedSizeZ += PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_SizePoint()->z = m_SpeedSizeZ;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedSizezDown()
{
	UpdateData(true);
	m_SpeedSizeZ -= PosRotSpeed;
#ifdef MeshEffect
	CMgr_EffectMesh::GetInstance()->Get_Speed_SizePoint()->z = m_SpeedSizeZ;
#endif 
	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedAlphaUp()
{
	UpdateData(true);
	m_SpeedAlpha += AlphaSpeed;
#ifdef MeshEffect
	* (CMgr_EffectMesh::GetInstance()->Get_Speed_Alpha()) = m_SpeedAlpha;
#endif 

	UpdateData(false);
}


void EffectTool::OnBnClickedSpeedAlphaDown()
{
	UpdateData(true);
	m_SpeedAlpha -= AlphaSpeed;
#ifdef MeshEffect
	* (CMgr_EffectMesh::GetInstance()->Get_Speed_Alpha()) = m_SpeedAlpha;
#endif 

	UpdateData(false);
}
