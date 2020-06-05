#pragma once

// EffectTool 대화 상자

class CMainFrame; 
class CMfcEffectToolView; 

class EffectTool : public CDialogEx
{
	DECLARE_DYNAMIC(EffectTool)

public:
	EffectTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EffectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EffectTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CListBox m_ListBoxTexture;
	CListBox m_ListBoxMesh;

	float m_CurrentPosX;
	float m_CurrentPosY;
	float m_CurrentPosZ;

	//frist value ---------------------------
	float m_FristValue_PosX;
	float m_FristValue_PosY;
	float m_FristValue_PosZ;
	float m_FirstValue_RotX;
	float m_FirstValue_RotY;
	float m_FirstValue_RotZ;
	float m_FirstValue_CX;
	float m_FirstValue_CY;
	float m_FirstValue_CZ;
	//final value ---------------------------
	float m_FinalValue_PosX;
	float m_FinalValue_PosY;
	float m_FinalValue_PosZ;
	float m_FinalValue_RotX;
	float m_FinalValue_RotY;
	float m_FinalValue_RotZ;
	float m_FinalValue_CX;
	float m_FinalValue_CY;
	float m_FinalValue_CZ;
	//-----------------------------------------

	CMainFrame* pMainFrame = nullptr; 
	CMfcEffectToolView* pForm = nullptr; 

	//-----------------------------------------
	afx_msg void OnBnClickedFristvaluePosxUp();
	afx_msg void OnBnClickedFristvaluePosxDown();
	afx_msg void OnBnClickedFristvaluePosyUp();
	afx_msg void OnBnClickedFristvaluePosyDown();
	afx_msg void OnBnClickedFristvaluePoszUp();
	afx_msg void OnBnClickedFristvaluePoszDown();
	afx_msg void OnBnClickedFristvalueRotxUp();
	afx_msg void OnBnClickedFristvalueRotxDown();
	afx_msg void OnBnClickedFristvalueRotyUp();
	afx_msg void OnBnClickedFristvalueRotyDown();
	afx_msg void OnBnClickedFristvalueRotzUp();
	afx_msg void OnBnClickedFristvalueRotzDown();
	afx_msg void OnBnClickedFristvalueCxUp();
	afx_msg void OnBnClickedFristvalueCxDown();
	afx_msg void OnBnClickedFristvalueCyUp();
	afx_msg void OnBnClickedFristvalueCyDown();
	afx_msg void OnBnClickedFristvalueCzUp();
	afx_msg void OnBnClickedFristvalueCzDowns();

	//----------------------------------------
	afx_msg void OnBnClickedFinalvaluePosxUp();
	afx_msg void OnBnClickedFinalvaluePosxDown();
	afx_msg void OnBnClickedFinalvaluePosyUp();
	afx_msg void OnBnClickedFinalvaluePosyDown();
	afx_msg void OnBnClickedFinalvaluePoszUp();
	afx_msg void OnBnClickedFinalvaluePoszDown();
	afx_msg void OnBnClickedFinalvalueRotxUp();
	afx_msg void OnBnClickedFinalvalueRotxDown();
	afx_msg void OnBnClickedFinalvalueRotyUp();
	afx_msg void OnBnClickedFinalvalueRotyDown();
	afx_msg void OnBnClickedFinalvalueRotzUp();
	afx_msg void OnBnClickedFinalvalueRotzDown();
	afx_msg void OnBnClickedFinalvalueCxUp();
	afx_msg void OnBnClickedFinalvalueCxDown();
	afx_msg void OnBnClickedFinalvalueCyUp();
	afx_msg void OnBnClickedFinalvalueCyDown();
	afx_msg void OnBnClickedFinalvalueCzUp();
	afx_msg void OnBnClickedFinalvalueCzDown();


	afx_msg void OnBnClickedTexturecreate();
	afx_msg void OnBnClickedMeshcreate();
	afx_msg void OnBnClickedAnimationplay();
	afx_msg void OnBnClickedAnimationstop();

	//==================================================
	float m_AlphaValue;
	afx_msg void OnBnClickedAlphavalueUp();
	afx_msg void OnBnClickedAlphavalueDown();
	afx_msg void OnBnClickedAlphavalueUp2();
	afx_msg void OnBnClickedAlphavalueDown2();
	float m_FinalValue_alpha;
	afx_msg void OnBnClickedButton40();
	float m_SpeedPosX;
	float m_SpeedPosY;
	float m_SpeedPosZ;
	float m_SpeedRotX;
	float m_SpeedRotY;
	float m_SpeedRotZ;
	float m_SpeedSizeX;
	float m_SpeedSizeY;
	float m_SpeedSizeZ;
	float m_SpeedAlpha;
	afx_msg void OnBnClickedSpeedPosxUp();
	afx_msg void OnBnClickedSpeedPosxDown();
	afx_msg void OnBnClickedSpeedPosyUp();
	afx_msg void OnBnClickedSpeedPosyDown();
	afx_msg void OnBnClickedSpeedPoszUp();
	afx_msg void OnBnClickedSpeedRotxUp();
	afx_msg void OnBnClickedSpeedPoszDown();
	afx_msg void OnBnClickedSpeedRotxDown();
	afx_msg void OnBnClickedSpeedRotyUp();
	afx_msg void OnBnClickedSpeedRotyDown();
	afx_msg void OnBnClickedSpeedRotzUp();
	afx_msg void OnBnClickedSpeedRotzDown();
	afx_msg void OnBnClickedSpeedSizexUp();
	afx_msg void OnBnClickedSpeedSizexDown();
	afx_msg void OnBnClickedSpeedSizeyUp();
	afx_msg void OnBnClickedSpeedSizeyDown();
	afx_msg void OnBnClickedSpeedSizezUp();
	afx_msg void OnBnClickedSpeedSizezDown();
	afx_msg void OnBnClickedSpeedAlphaUp();
	afx_msg void OnBnClickedSpeedAlphaDown();
	float m_UpdateTime;
};
