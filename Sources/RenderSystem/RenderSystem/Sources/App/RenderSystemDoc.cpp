// D3DRendererTestAppDoc.cpp : implementation of the CD3DRendererTestAppDoc class
//

#include "RenderSystem.h"
#include "RenderSystemDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppDoc

IMPLEMENT_DYNCREATE(CD3DRendererTestAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CD3DRendererTestAppDoc, CDocument)
	//{{AFX_MSG_MAP(CD3DRendererTestAppDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppDoc construction/destruction

CD3DRendererTestAppDoc::CD3DRendererTestAppDoc()
{
	// TODO: add one-time construction code here

}

CD3DRendererTestAppDoc::~CD3DRendererTestAppDoc()
{
}

BOOL CD3DRendererTestAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppDoc serialization

void CD3DRendererTestAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppDoc diagnostics

#ifdef _DEBUG
void CD3DRendererTestAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CD3DRendererTestAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppDoc commands
