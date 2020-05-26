// EffectTool2.cpp: 구현 파일
//

#include "pch.h"
#include "MfcEffectTool.h"
#include "EffectTool2.h"
#include "afxdialogex.h"


// EffectTool2 대화 상자

IMPLEMENT_DYNAMIC(EffectTool2, CDialogEx)

EffectTool2::EffectTool2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EffectTool2, pParent)
{

}

EffectTool2::~EffectTool2()
{
}

void EffectTool2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EffectTool2, CDialogEx)
END_MESSAGE_MAP()


// EffectTool2 메시지 처리기
