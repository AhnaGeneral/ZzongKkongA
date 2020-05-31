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

	float m_CurrentPosX;
	float m_CurrentPosY;
	float m_CurrentPosZ;

	//frist value ---------------------------
	float m_FristValue_PosX;
	float m_FristValue_PosY;
	float m_FristValue_PosZ;
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
	//---------------------------------------
	CMainFrame* pMainFrame = nullptr; 
	CMfcEffectToolView* pForm = nullptr; 

	afx_msg void OnBnClickedFristvaluePosxUp();
	afx_msg void OnBnClickedFristvaluePosxDown();
	afx_msg void OnBnClickedFristvaluePosyUp();
	afx_msg void OnBnClickedFristvaluePosyDown();
	afx_msg void OnBnClickedFristvaluePoszUp();
	afx_msg void OnBnClickedFristvaluePoszDown();
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

};
