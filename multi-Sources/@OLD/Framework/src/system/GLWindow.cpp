// GLWindow_w32.cpp
//
#include "GLWindow.h"
#include "GLContext.h"

#include "application/Application.h"

#include	<iostream>
#include	<vector>

using namespace s2Fwk;

namespace {
	std::string className = "S2Window";
}

// ------------------------------------------------------------------------------------------------
GLWindow::GLWindow( const std::string &title )
: _hwnd(0),
  _windowTitle(title)
{
	createWindow();
	createContext();
}

// ------------------------------------------------------------------------------------------------
GLWindow::~GLWindow()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
void  GLWindow::resize( int w, int h )
{
    //RECT r = {0, 0, w, h};
    //AdjustWindowRect(&r, GetWindowLong(_hwnd, GWL_STYLE), false);
    
	_width  = w;
	_height = h;

//    SetWindowPos(_hwnd, 0, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);

	// @todo: send resize event to gl context?
}

// ------------------------------------------------------------------------------------------------
void GLWindow::show( bool visible )
{
	ShowWindow(_hwnd, visible ? SW_SHOW : SW_HIDE);
}

// ------------------------------------------------------------------------------------------------
void GLWindow::createWindow()
{
	WNDCLASS wc;

	// Register new class window
	if(!GetClassInfo(GetModuleHandle(0),className.c_str(),&wc))
	{
		//wc.style			= CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window
		wc.style			= 0;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= GetModuleHandle(0);
		wc.hIcon			= 0;
		wc.hCursor			= LoadCursor(0, IDC_ARROW);
		wc.hbrBackground	= 0;
		wc.lpfnWndProc		= GLWindow::WndProc;
		wc.lpszMenuName		= 0;
		wc.lpszClassName	= className.c_str();

		RegisterClass(&wc);
	}


	DWORD flags;
	
	// default size
	_width  = 400;
	_height = 300;

	//if( windowed )
	{
		flags     = WS_POPUP /*| WS_VISIBLE*/ | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX;
		_windowed = true;
	//	desiredW = windowWidth;
	//	desiredH = windowHeight;
	}
	//else
	//{
	//	flags    = WS_POPUP /*| WS_VISIBLE*/ | WS_MAXIMIZE| WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	//	desiredW = CW_USEDEFAULT;
	//	desiredH = CW_USEDEFAULT;
	//}

	_hwnd = CreateWindow( className.c_str(),
						  _windowTitle.c_str(),
						  flags,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  _width, _height,
						  0, 0,GetModuleHandle(0), this );

	std::cout << "Window created" << std::endl;
}

// ------------------------------------------------------------------------------------------------
void GLWindow::createContext()
{
	_context = new GLContext( (unsigned int)_hwnd );
	_context->makeCurrent();
}

// ------------------------------------------------------------------------------------------------
void GLWindow::update()
{
	_context->swapBuffers();
}

// ------------------------------------------------------------------------------------------------
void GLWindow::validateResolution()
{
	DEVMODE settings;
	ZeroMemory(&settings,sizeof(DEVMODE));

	settings.dmSize			= sizeof(DEVMODE);
	settings.dmPelsWidth	= _width;
	settings.dmPelsHeight	= _height;
//	settings.dmBitsPerPel	= windowBpp;
	settings.dmFields		= DM_PELSWIDTH | DM_PELSHEIGHT; /*| DM_BITSPERPEL;*/

//	LONG r = ChangeDisplaySettings(&settings, windowed ? CDS_TEST : CDS_FULLSCREEN );


	//if(bFullscreen)
	//{

	//	LONG result = ChangeDisplaySettings(&settings, CDS_TEST);

	//	if (result != DISP_CHANGE_SUCCESSFUL)
	//		bFullscreen = false;	// FullScreen non supportato
	//}

	//ChangeDisplaySettings(&settings, CDS_FULLSCREEN);
}


// ------------------------------------------------------------------------------------------------
void GLWindow::destroy()
{
	UnregisterClass(className.c_str(),GetModuleHandle(0));

	if( _hwnd )
		DestroyWindow(_hwnd);

	//if(!windowed)
	//ChangeDisplaySettings(0,0);

	//if( contextGL )
	//	delete contextGL;

	_hwnd      = 0;
	//contextGL = 0;

	//if(!bValid)
	//	return;

	//if(inputManager)
	//{
	//	if(kb)		inputManager->destroyInputObject(kb);
	//	if(mouse)	inputManager->destroyInputObject(mouse);
	//
	//	OIS::InputManager::destroyInputSystem(inputManager);
	//	inputManager = 0;
	//	kb					 = 0;
	//	mouse				 = 0;
	//}


	//if(frame)
	//{
	//	frame->destroyContext();
	//	delete frame;
	//	frame = 0;
	//}

	//if(hWindow && !DestroyWindow(hWindow))
	//	std::cout << "Error closing window\n";

	//hWindow = 0;

	//if(!UnregisterClass(CLASS_NAME.c_str(),GetModuleHandle(0)))
	//	std::cout << "Error unregistering window class\n";

	//if(bFullscreen)
	//	ChangeDisplaySettings(0,0);

	//std::cout << "MainWindow destroyed..\n";
}

// ------------------------------------------------------------------------------------------------
bool GLWindow::processWinMessages()
{
	MSG		msg;

	if( PeekMessage( &msg, 0, 0U, 0U, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return msg.message != WM_QUIT;
}

// ------------------------------------------------------------------------------------------------
LRESULT CALLBACK GLWindow::WndProc(HWND hWnd,unsigned int uMsg,WPARAM wParam,LPARAM lParam)
{
	switch( uMsg )
	{
	case WM_SIZE:
		Application::s2App()->resizeWindow( LOWORD(lParam),HIWORD(lParam) );
		//switch (wParam)
		//{
		//	case SIZE_MINIMIZED:
		//		staticThis->bActive = false;
		//		staticThis->bMinimized = true;
		//		break;

		//	case SIZE_RESTORED:
		//		staticThis->bActive = true;
		//		staticThis->bMinimized = false;
		//		break;
		//}
		break;
		
		//case WM_SETCURSOR:
		//	if(staticThis->bFullscreen)
		//		SetCursor(0);
		//	else
		//		SetCursor(LoadCursor(0,IDC_ARROW));
		//	break;


	//case WM_SYSCOMMAND:
	//	if(!windowed)
	//	{
	//		switch(wParam)
	//		{
	//		case SC_SCREENSAVE:
	//		case SC_MONITORPOWER:
	//			return 0;
	//		}
	//	}
	//	break;


	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ------------------------------------------------------------------------------------------------
void GLWindow::setWindowed( bool on )
{
	_windowed = on;
}


// ------------------------------------------------------------------------------------------------
int          GLWindow::width() const  { return _width; }   
int          GLWindow::height() const { return _height; }   
unsigned int GLWindow::hwnd() const   { return (unsigned int)_hwnd;}
