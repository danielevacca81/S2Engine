// D3DRendererTestApp.h : main header file for the D3DRENDERERTESTAPP application
//

#if !defined(AFX_D3DRENDERERTESTAPP_H__ED1E5A17_A0A0_4AEE_BC01_9DB1FED5886F__INCLUDED_)
#define AFX_D3DRENDERERTESTAPP_H__ED1E5A17_A0A0_4AEE_BC01_9DB1FED5886F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>         // MFC core and standard components

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppApp:
// See D3DRendererTestApp.cpp for the implementation of this class
//

class CD3DRendererTestAppApp : public CWinApp
{
public:
	CD3DRendererTestAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD3DRendererTestAppApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CD3DRendererTestAppApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D3DRENDERERTESTAPP_H__ED1E5A17_A0A0_4AEE_BC01_9DB1FED5886F__INCLUDED_)
