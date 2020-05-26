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
{

}

EffectTool::~EffectTool()
{
}

void EffectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EffectTool, CDialogEx)
END_MESSAGE_MAP()


// EffectTool 메시지 처리기
