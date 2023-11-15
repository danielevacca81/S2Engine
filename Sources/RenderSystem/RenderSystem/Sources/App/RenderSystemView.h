// D3DRendererTestAppView.h : interface of the CD3DRendererTestAppView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_D3DRENDERERTESTAPPVIEW_H__B8F4D2AC_CE4C_4510_A01D_92C011E353B4__INCLUDED_)
#define AFX_D3DRENDERERTESTAPPVIEW_H__B8F4D2AC_CE4C_4510_A01D_92C011E353B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>         // MFC core and standard components
#include <windows.h>

#include "../RenderSystem/RenderDevice.h"
#include "../RenderSystem/Sphere.h"

class CD3DRendererTestAppView : public CView
{
protected:
	UINT pRefreshTimer;

	RenderDevice*	pRenderDevice;
	Sphere*			pSphere;
	Texture*		pTexture;

	bool m_bUseOpenGL;

	bool CreateRenderer();
	void ReleaseRenderer();
	bool SwapRenderers();

	bool CreateVertexBuffer();
	bool CreateTexture( DWORD dwGenMethod=0 );

protected: // create from serialization only
	CD3DRendererTestAppView();
	DECLARE_DYNCREATE(CD3DRendererTestAppView)

// Attributes
public:
	CD3DRendererTestAppDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD3DRendererTestAppView)
	protected:
	virtual void OnDraw(CDC* pDC);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CD3DRendererTestAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:


// Generated message map functions
protected:
	void UpdateMainFrameStatusBar();
	//{{AFX_MSG(CD3DRendererTestAppView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnViewDirectx();
	afx_msg void OnUpdateViewDirectx(CCmdUI* pCmdUI);
	afx_msg void OnViewOpengl();
	afx_msg void OnUpdateViewOpengl(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSwitchToD3d();
	afx_msg void OnSwitchToOgl();
	afx_msg void OnUpdateSwitchToD3d(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchToOgl(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTexturePlasma();
	afx_msg void OnUpdateTexturePlasma(CCmdUI* pCmdUI);
	afx_msg void OnTextureChecker();
	afx_msg void OnUpdateTextureChecker(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in D3DRendererTestAppView.cpp
inline CD3DRendererTestAppDoc* CD3DRendererTestAppView::GetDocument()
   { return (CD3DRendererTestAppDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D3DRENDERERTESTAPPVIEW_H__B8F4D2AC_CE4C_4510_A01D_92C011E353B4__INCLUDED_)
