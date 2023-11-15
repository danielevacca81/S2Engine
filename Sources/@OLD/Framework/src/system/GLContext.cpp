// GLFrame_w32.cpp
//
#include "GLContext.h"

#include <GL/gl.h>

#include <string>
#include <iostream>

using namespace s2Fwk;

// ------------------------------------------------------------------------------------------------
GLContext::GLContext( unsigned int hWnd )
: hwnd((HWND)hWnd)
{
	create();
	makeCurrent();
	initExtensions();
}

// ------------------------------------------------------------------------------------------------
GLContext::~GLContext()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
void GLContext::create()
{
	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd,0,sizeof(pfd));

	pfd.nSize	    = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType	= PFD_TYPE_RGBA;
	pfd.dwLayerMask	= PFD_MAIN_PLANE;
	pfd.cColorBits  = 32;
	pfd.cDepthBits  = 24;

	hDC = GetDC(hwnd);
	if(hDC == NULL)
	{
		destroy();
		return;
	}

	int pxlfmt = ChoosePixelFormat(hDC,&pfd);
	if(!pxlfmt || !SetPixelFormat(hDC,pxlfmt,&pfd))
	{
		destroy();
		return;
	}
	
	if(!(hRC = wglCreateContext(hDC)))
	{
		destroy();
		return;
	}
}

// ------------------------------------------------------------------------------------------------
void GLContext::destroy()
{
	if(hRC)
	{
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(hRC);
		hRC = NULL;
	}

	if(hDC && !ReleaseDC(hwnd,hDC))
		hDC = NULL;
}

// ------------------------------------------------------------------------------------------------
void GLContext::makeCurrent()
{
	wglMakeCurrent(hDC,hRC);
}

// ------------------------------------------------------------------------------------------------
void GLContext::swapBuffers()
{
	SwapBuffers(hDC);
}

// ------------------------------------------------------------------------------------------------
void GLContext::initExtensions()
{
	//if( glewInit() == GLEW_OK )
	//	std::cout << "GLEW Initialized properly" << std::endl;
}

// ------------------------------------------------------------------------------------------------
void GLContext::enableVSync(bool enabled)
{
	//if( glewIsSupported("wglSwapIntervalEXT") )
       // wglSwapIntervalEXT(enabled ? 1 : 0);
}