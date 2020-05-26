#pragma once


// EffectTool 대화 상자

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
};
