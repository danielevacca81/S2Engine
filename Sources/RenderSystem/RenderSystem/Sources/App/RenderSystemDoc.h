// D3DRendererTestAppDoc.h : interface of the CD3DRendererTestAppDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_D3DRENDERERTESTAPPDOC_H__037EB3F3_A591_478F_937A_CFA4642DAC2D__INCLUDED_)
#define AFX_D3DRENDERERTESTAPPDOC_H__037EB3F3_A591_478F_937A_CFA4642DAC2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CD3DRendererTestAppDoc : public CDocument
{
protected: // create from serialization only
	CD3DRendererTestAppDoc();
	DECLARE_DYNCREATE(CD3DRendererTestAppDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD3DRendererTestAppDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CD3DRendererTestAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CD3DRendererTestAppDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D3DRENDERERTESTAPPDOC_H__037EB3F3_A591_478F_937A_CFA4642DAC2D__INCLUDED_)
