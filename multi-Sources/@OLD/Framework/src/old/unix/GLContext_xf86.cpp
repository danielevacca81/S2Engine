// GLContext_xf86.cpp
//
#include "GLContext_xf86.h"

//#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>

using namespace s2Core;

// ------------------------------------------------------------------------------------------------
GLContext::GLContext( unsigned int hWnd )
: window((Window)hWnd)
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
void GLContext::initExtensions()
{
    //if( glewInit() == GLEW_OK )
	//	std::cout << "GLEW Initialized properly" << std::endl;
}

// ------------------------------------------------------------------------------------------------
void GLContext::create()
{
    //glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    //glXMakeCurrent(dpy, win, glc);
    Status XGetWindowAttributes(display, window, window_attributes_return);
	ctx = glXCreateContext(dpy, vi, NULL, true);
}

// ------------------------------------------------------------------------------------------------
void GLContext::makeCurrent()
{
	glXMakeCurrent(dpy, window, ctx);
}

// ------------------------------------------------------------------------------------------------
void GLContext::destroy()
{
	/*if(ctx)
	{
		glXMakeCurrent(dc, None, NULL);
		glXDestroyContext(dc, ctx);
		ctx = NULL;
	}

	if(dc)
	{
		XCloseDisplay(dc);
		dc = NULL;
	}*/
}
