// D3DRendererTestAppView.cpp : implementation of the CD3DRendererTestAppView class
//

#include "RenderSystem.h"
#include "RenderSystemDoc.h"
#include "RenderSystemView.h"

#include "MainFrm.h"
#include <cassert>
#include <math.h>

#include "mmsystem.h" // timeGetTime()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppView

IMPLEMENT_DYNCREATE(CD3DRendererTestAppView, CView)

BEGIN_MESSAGE_MAP(CD3DRendererTestAppView, CView)
	//{{AFX_MSG_MAP(CD3DRendererTestAppView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_DIRECTX, OnViewDirectx)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DIRECTX, OnUpdateViewDirectx)
	ON_COMMAND(ID_VIEW_OPENGL, OnViewOpengl)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPENGL, OnUpdateViewOpengl)
	ON_WM_CHAR()
	ON_COMMAND(ID_SWITCH_TO_D3D, OnSwitchToD3d)
	ON_COMMAND(ID_SWITCH_TO_OGL, OnSwitchToOgl)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_TO_D3D, OnUpdateSwitchToD3d)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_TO_OGL, OnUpdateSwitchToOgl)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(ID_TEXTURE_PLASMA, OnTexturePlasma)
	ON_UPDATE_COMMAND_UI(ID_TEXTURE_PLASMA, OnUpdateTexturePlasma)
	ON_COMMAND(ID_TEXTURE_CHECKER, OnTextureChecker)
	ON_UPDATE_COMMAND_UI(ID_TEXTURE_CHECKER, OnUpdateTextureChecker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppView construction/destruction

CD3DRendererTestAppView::CD3DRendererTestAppView()
{
	pRenderDevice = NULL;
	pSphere		  = NULL;
	pTexture	  = NULL;

	m_bUseOpenGL  = false;

#ifndef COMPILE_OPENGL_IMPLEMENTATION
	m_bUseOpenGL  = true;
#endif

#ifndef COMPILE_DIRECT3D_IMPLEMENTATION
	m_bUseOpenGL  = false;
#endif
}

CD3DRendererTestAppView::~CD3DRendererTestAppView()
{
	ReleaseRenderer();
}

bool CD3DRendererTestAppView::CreateRenderer()
{
	assert( (pRenderDevice==NULL) && "CD3DRendererTestAppView::CreateRenderer() --> Attempting to create renderer while this one is not NULL." );

	CClientDC dc(this);

	RECT rc;
	GetClientRect( &rc );

	// Build renderer
	//--
	DWORD width  = ( rc.right-rc.left<=0 ? 1024 : rc.right-rc.left );
	DWORD height = ( rc.bottom-rc.top<=0 ? 1024 : rc.bottom-rc.top );
	HWND  hWnd   = GetSafeHwnd();

	pRenderDevice = new RenderDevice();

	if ( !m_bUseOpenGL )
	{
#ifdef COMPILE_DIRECT3D_IMPLEMENTATION
		pRenderDevice->UseD3D( hWnd, width, height );
#else
		pRenderDevice->UseOpenGL( hWnd, width, height );
#endif
	}
	else
	{
#ifdef COMPILE_OPENGL_IMPLEMENTATION
		pRenderDevice->UseOpenGL( hWnd, width, height );
#else
		pRenderDevice->UseD3D( hWnd, width, height );
#endif
	}

	// Now put the renderer in some initial state
	//--
	// Build a view and a projection matrix
	//--
	Matrix4x4 proj;
	proj.SetPerspective(0.1f, 1000.0f, 45.0f, (float)(width)/(float)(height) );
	pRenderDevice->MatrixModeSet( RSMM_PROJECTION );
	pRenderDevice->MatrixLoad( proj );
	pRenderDevice->MatrixModeSet( RSMM_MODELVIEW );
	pRenderDevice->MatrixLoadIdentity();

	pRenderDevice->SetRenderState( RSRS_LIGHTINGENABLE, 0 );
	pRenderDevice->SetRenderState( RSRS_CULLINGENABLE, 1 );
	pRenderDevice->SetRenderState( RSRS_FILLMODE, RSFILL_SOLID );

	return true;
}

bool CD3DRendererTestAppView::CreateVertexBuffer()
{
	if (pRenderDevice!=NULL)
	{
		pSphere = new Sphere( pRenderDevice, 1.5f, 50 );
	}
	else
	{
		return false;
	}

	return true;
}


bool CD3DRendererTestAppView::CreateTexture( DWORD dwGenMethod )
{
	// Create texture
	//--
	pTexture = pRenderDevice->CreateTexture( 128, 128, RSFMT_A8R8G8B8, false, 1 );

	// Fill it with some data
	//--
	if ( pTexture->Lock() )
	{
		DWORD i=0, j=0;
		DWORD *pPixels = (DWORD*)pTexture->GetData();
		DWORD dwPitch  = pTexture->GetPitch();
		DWORD dwWidth  = pTexture->GetWidth();
		DWORD dwHeight = pTexture->GetHeight();

		if ( dwGenMethod==1 )
		{
			for ( j=0; j<dwHeight; j++ )
			{
				for (i=0; i<dwWidth; i++)
				{
					BYTE r=0;
					BYTE g=0;
					BYTE b=0;

					// Do a kind of plasma
					//--
					r = (BYTE)( 0.9f*(127.0f + 127.0f*cosf( 5*j*3.1415f/dwWidth )*sinf( 2*i*3.1415f/dwWidth ) ));
					g = (BYTE)( 0.8f*(127.0f + 127.0f*sinf( 0.2f*j*3.1415f/dwWidth + i*3.1415f/dwWidth )*sinf( 2*j*i*3.1415f/(dwWidth*dwWidth) - 0.18f )*cosf( 2*(j+i)*3.1415f/dwWidth - 0.18f )) );
					b = (BYTE)( 0.6f*(127.0f + 127.0f*sinf( 3*i*3.1415f/dwWidth )*sinf( 2*j*3.1415f/dwWidth - 0.18f )*sinf( 2*j*3.1415f/dwWidth - 0.18f ) ) );

					pTexture->SetPixel4uc( 255, r, g, b );
				}
			}
		}
		else if ( dwGenMethod==0 )
		{
			for ( j=0; j<dwHeight; j++ )
			{
				for (i=0; i<dwWidth; i++)
				{
					BYTE r=0;
					BYTE g=0;
					BYTE b=0;

					// Do a 2x2 checker
					//--
					if ( i < dwWidth/2 )
						r=0;
					else
						r=255;

					if ( j < dwHeight/2 )
						g=255;
					else
						g=0;

					pTexture->SetPixel4uc( 255, r, g, b );
				}

				pPixels += (dwPitch/4-dwWidth);
			}
		}

		pTexture->Unlock();
	}
	else
	{
		return false;
	}

	return true;
}


bool CD3DRendererTestAppView::SwapRenderers()
{
	// First release renderer
	//--
	ReleaseRenderer();

	// Then Update flag
	//--
	m_bUseOpenGL = !m_bUseOpenGL;

	// Finally create new renderer
	//--
	bool bSucceeded = false;
	bSucceeded = CreateRenderer();
	bSucceeded = bSucceeded && CreateVertexBuffer();
	bSucceeded = bSucceeded && CreateTexture();

	// Update the status bar of main frame window and repaint the view
	//--
	UpdateMainFrameStatusBar();

	return bSucceeded;
}


void CD3DRendererTestAppView::ReleaseRenderer()
{
	SAFE_DELETE( pTexture );
	SAFE_DELETE( pSphere );
	SAFE_DELETE( pRenderDevice );
}


/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppView diagnostics

#ifdef _DEBUG
void CD3DRendererTestAppView::AssertValid() const
{
	CView::AssertValid();
}

void CD3DRendererTestAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CD3DRendererTestAppDoc* CD3DRendererTestAppView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CD3DRendererTestAppDoc)));
	return (CD3DRendererTestAppDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CD3DRendererTestAppView message handlers

int CD3DRendererTestAppView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= (CS_OWNDC | CS_HREDRAW /*| CS_DRAW */ );

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create timer
	//--
	pRefreshTimer = SetTimer( 69, 10, 0 );
	
	// Create renderer
	//--
	SwapRenderers();

	return 0;
}

void CD3DRendererTestAppView::OnDestroy() 
{
	KillTimer( 69 );
	CView::OnDestroy();	
}

void CD3DRendererTestAppView::OnDraw(CDC* pDC) 
{	
	RECT clientRect;
	GetClientRect( &clientRect );

	int width  = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	if ( pRenderDevice==NULL )
	{
		pDC->SetBkColor( 0xFFFF0000 );
	}
	else
	{
		// Start drawing the scene
		//--
		pRenderDevice->BeginRendering();

		// Clear screen
		//--
		pRenderDevice->SetViewport( 0, 0, width, height );
		pRenderDevice->SetBackgroundColor( 1, 0, 0, 0 );
		pRenderDevice->Clear( RSCLR_COLOR | RSCLR_ZBUFFER );

		// Set view transformation
		//--
		pRenderDevice->MatrixModeSet( RSMM_MODELVIEW );
		pRenderDevice->MatrixLoadIdentity();
		pRenderDevice->MatrixTranslation( 1, 0, -5.0f );

		// Draw vertex buffer content 4 times
		//--
		pRenderDevice->SetViewport( 0, 0, width/2, height/2 );
		pRenderDevice->MatrixRotation( timeGetTime()/30.0f, 0, 0, 1 );
		pRenderDevice->SetBackgroundColor( 1, 1, 0, 0 );
		pRenderDevice->Clear( RSCLR_COLOR | RSCLR_ZBUFFER );
		pRenderDevice->UseTexture( pTexture );
		pSphere->Draw( pRenderDevice );

		pRenderDevice->SetViewport( width/2, height/2, width/2, height/2 );
		pRenderDevice->MatrixRotation( timeGetTime()/30.0f, 0, 0.5, 0.5 );
		pRenderDevice->SetBackgroundColor( 1, 1, 1, 0 );
		pRenderDevice->Clear( RSCLR_COLOR | RSCLR_ZBUFFER );
		pRenderDevice->UseTexture( pTexture );
		pSphere->Draw( pRenderDevice );
		
		pRenderDevice->SetViewport( width/2, 0, width/2, height/2 );
		pRenderDevice->MatrixRotation( timeGetTime()/30.0f, 1, 0, 0 );
		pRenderDevice->SetBackgroundColor( 1, 1, 0, 1 );
		pRenderDevice->Clear( RSCLR_COLOR | RSCLR_ZBUFFER );
		pRenderDevice->UseTexture( pTexture );
		pSphere->Draw( pRenderDevice );

		pRenderDevice->SetViewport( 0, height/2, width/2, height/2 );
		pRenderDevice->MatrixRotation( timeGetTime()/30.0f, 0, 1, 0 );
		pRenderDevice->SetBackgroundColor( 1, 0, 1, 0 );
		pRenderDevice->Clear( RSCLR_COLOR | RSCLR_ZBUFFER );
		pRenderDevice->UseTexture( pTexture );
		pSphere->Draw( pRenderDevice );

		// Finished drawing the scene
		//--
		pRenderDevice->EndRendering();

		// Display backbuffer
		//--
		pRenderDevice->SwapBuffers();
	}

	CView::OnDraw( pDC );
}

void CD3DRendererTestAppView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	CView::CalcWindowRect(lpClientRect, nAdjustType);
}

void CD3DRendererTestAppView::OnViewDirectx() 
{
	// Do something only if we are in OpenGL mode
	//--
	if ( m_bUseOpenGL )
	{
		SwapRenderers();

		CWnd* pMain = AfxGetMainWnd();
		pMain->SendMessage( WM_ERASEBKGND );
		pMain->SendMessage( WM_PAINT );
		PostMessage( WM_PAINT );
	}
}

void CD3DRendererTestAppView::OnUpdateViewDirectx(CCmdUI* pCmdUI) 
{
	// Update menu
	//--
	CWnd* pMain = AfxGetMainWnd();
	CMenu* pMenu = ( pMain==NULL ? NULL : pMain->GetMenu()->GetSubMenu(1) );

	if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
	{
		if (!m_bUseOpenGL)
		{
			pMenu->CheckMenuItem( ID_VIEW_DIRECTX, MF_CHECKED | MF_BYCOMMAND );
			pMenu->CheckMenuItem( ID_VIEW_OPENGL, MF_UNCHECKED | MF_BYCOMMAND );
		}
		else
		{
			pMenu->CheckMenuItem( ID_VIEW_DIRECTX, MF_UNCHECKED | MF_BYCOMMAND );
			pMenu->CheckMenuItem( ID_VIEW_OPENGL, MF_CHECKED | MF_BYCOMMAND );
		}
	}

	pMain->DrawMenuBar();
}

void CD3DRendererTestAppView::OnViewOpengl() 
{
	// Do something only if we are in OpenGL mode
	//--
	if ( !m_bUseOpenGL )
	{
		SwapRenderers();

		CWnd* pMain = AfxGetMainWnd();
		pMain->SendMessage( WM_ERASEBKGND );
		pMain->SendMessage( WM_PAINT );
		PostMessage( WM_PAINT );
	}
}

void CD3DRendererTestAppView::OnUpdateViewOpengl(CCmdUI* pCmdUI) 
{
	// Update menu
	//--
	CWnd* pMain = AfxGetMainWnd();
	CMenu* pMenu = ( pMain==NULL ? NULL : pMain->GetMenu()->GetSubMenu(1) );

	if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
	{
		if (!m_bUseOpenGL)
		{
			pMenu->CheckMenuItem( ID_VIEW_DIRECTX, MF_CHECKED | MF_BYCOMMAND );
			pMenu->CheckMenuItem( ID_VIEW_OPENGL, MF_UNCHECKED | MF_BYCOMMAND );
		}
		else
		{
			pMenu->CheckMenuItem( ID_VIEW_DIRECTX, MF_UNCHECKED | MF_BYCOMMAND );
			pMenu->CheckMenuItem( ID_VIEW_OPENGL, MF_CHECKED | MF_BYCOMMAND );
		}
	}

	pMain->DrawMenuBar();
}

void CD3DRendererTestAppView::UpdateMainFrameStatusBar()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	
	if ( m_bUseOpenGL )
	{
		pMain->SetStatusBarText( (pRenderDevice==NULL ? "Error creating OpenGL render device" : "Using OpenGL" ) );
	}
	else
	{
		pMain->SetStatusBarText( (pRenderDevice==NULL ? "Error creating Direct3D render device" : "Using Direct3D" ) );
	}

	pMain->PostMessage( WM_PAINT );
	PostMessage( WM_PAINT );
}

void CD3DRendererTestAppView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case 'w':
		pRenderDevice->SetRenderState( RSRS_FILLMODE, RSFILL_WIREFRAME );
		break;
	case 'W':
		pRenderDevice->SetRenderState( RSRS_FILLMODE, RSFILL_SOLID );
		break;
	case 'c':
		pRenderDevice->SetRenderState( RSRS_CULLINGENABLE, 0 );
		break;
	case 'C':
		pRenderDevice->SetRenderState( RSRS_CULLINGENABLE, 1 );
		break;
	case 'l':
		pRenderDevice->SetRenderState( RSRS_LIGHTINGENABLE, 0 );
		break;
	case 'L':
		pRenderDevice->SetRenderState( RSRS_LIGHTINGENABLE, 1 );
		break;
	}
	
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->PostMessage( WM_PAINT );
	PostMessage( WM_PAINT );

	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CD3DRendererTestAppView::OnSwitchToD3d() 
{
	OnViewDirectx();
}

void CD3DRendererTestAppView::OnSwitchToOgl() 
{
	OnViewOpengl();	
}

void CD3DRendererTestAppView::OnUpdateSwitchToD3d(CCmdUI* pCmdUI) 
{
	OnUpdateViewDirectx( pCmdUI );
}

void CD3DRendererTestAppView::OnUpdateSwitchToOgl(CCmdUI* pCmdUI) 
{
	OnUpdateViewOpengl( pCmdUI );
}

void CD3DRendererTestAppView::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent==69 ) // Refresh timer
	{
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->PostMessage( WM_PAINT );
		PostMessage( WM_PAINT );
	}
	
	CView::OnTimer(nIDEvent);
}

void CD3DRendererTestAppView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	// Rebuild the renderer (without swapping Api -> m_bUseOpenGL = !m_bUseOpenGL)
	//--
	m_bUseOpenGL = !m_bUseOpenGL;
	SwapRenderers();
}

void CD3DRendererTestAppView::OnTexturePlasma() 
{
	SAFE_DELETE( pTexture );
	CreateTexture( 0 );	
}

void CD3DRendererTestAppView::OnUpdateTexturePlasma(CCmdUI* pCmdUI) 
{
	// Update menu
	//--
	CWnd* pMain = AfxGetMainWnd();
	CMenu* pMenu = ( pMain==NULL ? NULL : pMain->GetMenu()->GetSubMenu(2) );

	if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
	{
		pMenu->CheckMenuItem( ID_TEXTURE_PLASMA, MF_CHECKED | MF_BYCOMMAND );
		pMenu->CheckMenuItem( ID_TEXTURE_CHECKER, MF_UNCHECKED | MF_BYCOMMAND );
	}

	pMain->DrawMenuBar();
}

void CD3DRendererTestAppView::OnTextureChecker() 
{
	SAFE_DELETE( pTexture );
	CreateTexture( 1 );
}

void CD3DRendererTestAppView::OnUpdateTextureChecker(CCmdUI* pCmdUI) 
{
	// Update menu
	//--
	CWnd* pMain = AfxGetMainWnd();
	CMenu* pMenu = ( pMain==NULL ? NULL : pMain->GetMenu()->GetSubMenu(2) );

	if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
	{
		pMenu->CheckMenuItem( ID_TEXTURE_PLASMA, MF_UNCHECKED | MF_BYCOMMAND );
		pMenu->CheckMenuItem( ID_TEXTURE_CHECKER, MF_CHECKED | MF_BYCOMMAND );
	}

	pMain->DrawMenuBar();	
}
