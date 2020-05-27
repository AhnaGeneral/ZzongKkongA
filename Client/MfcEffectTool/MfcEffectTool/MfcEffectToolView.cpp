
// MfcEffectToolView.cpp: CMfcEffectToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MfcEffectTool.h"
#endif

#include "MfcEffectToolDoc.h"
#include "MfcEffectToolView.h"

#include "MainFrm.h"
#include "MyForm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



HWND m_nHnd; 
HINSTANCE hInst;
// CMfcEffectToolView

IMPLEMENT_DYNCREATE(CMfcEffectToolView, CView)

BEGIN_MESSAGE_MAP(CMfcEffectToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMfcEffectToolView 생성/소멸

CMfcEffectToolView::CMfcEffectToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMfcEffectToolView::~CMfcEffectToolView()
{
}

BOOL CMfcEffectToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMfcEffectToolView 그리기

void CMfcEffectToolView::OnDraw(CDC* /*pDC*/)
{
	CMfcEffectToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return; 
	//gGameFramework.FrameAdvance();
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMfcEffectToolView 인쇄

BOOL CMfcEffectToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMfcEffectToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMfcEffectToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMfcEffectToolView 진단

#ifdef _DEBUG
void CMfcEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcEffectToolDoc* CMfcEffectToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcEffectToolDoc)));
	return (CMfcEffectToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMfcEffectToolView 메시지 처리기


void CMfcEffectToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	hInst = AfxGetInstanceHandle();
	m_nHnd = m_hWnd;

	gGameFramework.OnCreate(hInst, m_nHnd);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	MyForm* pForm = dynamic_cast<MyForm*>(pMainFrame->m_mainSplite.GetPane(0, 0));
	m_peffectTool1 = pForm->m_pEffectTool;

}


void CMfcEffectToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_peffectTool1->UpdateData(true);
	m_peffectTool1->m_CurrentPosX = point.x;
	m_peffectTool1->m_CurrentPosY = point.y;
	m_peffectTool1->UpdateData(false);

	CView::OnLButtonDown(nFlags, point);
}
