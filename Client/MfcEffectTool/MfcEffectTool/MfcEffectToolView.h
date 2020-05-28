﻿
// MfcEffectToolView.h: CMfcEffectToolView 클래스의 인터페이스
//

#pragma once
#include "MfcEffectToolDoc.h"
#include "GameFramework.h"
#include "EffectTool.h"


class CMfcEffectToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMfcEffectToolView() noexcept;
	DECLARE_DYNCREATE(CMfcEffectToolView)

// 특성입니다.
public:
	CMfcEffectToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMfcEffectToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	
public:
	CGameFramework *gGameFramework;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	EffectTool* m_peffectTool1;


};

#ifndef _DEBUG  // MfcEffectToolView.cpp의 디버그 버전
inline CMfcEffectToolDoc* CMfcEffectToolView::GetDocument() const
   { return reinterpret_cast<CMfcEffectToolDoc*>(m_pDocument); }
#endif

