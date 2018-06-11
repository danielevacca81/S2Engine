// Context.cpp
//
#include "Context.h"

#include "OpenGL.h"
#include "Extensions.h"

#include <windows.h>

#include <string>
#include <iostream>

using namespace s2;

using namespace s2::OpenGL;

// ------------------------------------------------------------------------------------------------
Context::Context( void *winID )
: _hwnd(0)
, _hDC(0)
, _hRC(0)
{
	create(winID);
	
	makeCurrent();
}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
void Context::create( void *winID )
{
	if( !winID )
		return;

	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd,0,sizeof(pfd));

	pfd.nSize	    = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType	= PFD_TYPE_RGBA;
	pfd.dwLayerMask	= PFD_MAIN_PLANE;
	pfd.cColorBits  = 32;
	pfd.cDepthBits  = 24;

	_hwnd = (HWND)winID;
	_hDC  = GetDC((HWND)_hwnd);	
	if(_hDC == NULL)
	{
		destroy();
		return;
	}

	int pxlfmt = ChoosePixelFormat((HDC)_hDC,&pfd);
	if(!pxlfmt || !SetPixelFormat((HDC)_hDC,pxlfmt,&pfd))
	{
		destroy();
		return;
	}
	
	if(!(_hRC = wglCreateContext((HDC)_hDC)))
	{
		destroy();
		return;
	}
}

// ------------------------------------------------------------------------------------------------
void Context::destroy()
{
	if(_hRC)
	{
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext((HGLRC)_hRC);
		_hRC = NULL;
	}

	if(_hDC && !ReleaseDC((HWND)_hwnd,(HDC)_hDC))
		_hDC = NULL;
}

// ------------------------------------------------------------------------------------------------
bool Context::initExtensions()
{
	return OpenGL::initExtensions();
}

// ------------------------------------------------------------------------------------------------
std::vector<std::string> Context::extensions() const
{
	return OpenGL::extensions();
}

// ------------------------------------------------------------------------------------------------
std::string Context::info() const
{
	return OpenGL::contextInfo();
}


// ------------------------------------------------------------------------------------------------
void Context::makeCurrent()
{
	if( _hDC && _hRC )
		wglMakeCurrent((HDC)_hDC,(HGLRC)_hRC);
}

// ------------------------------------------------------------------------------------------------
void Context::swapBuffers()
{
	if( _hDC )
		SwapBuffers((HDC)_hDC);
}

// ------------------------------------------------------------------------------------------------
void Context::enableVSync(bool enabled)
{
	//if( glewIsSupported("wglSwapIntervalEXT") )
       // wglSwapIntervalEXT(enabled ? 1 : 0);
}