// Context.cpp
//
#include "Context.h"

#include "OpenGL.h"
#include "Extensions.h"


#include <string>
#include <iostream>
#include <cassert>

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#endif


using namespace s2;
using namespace s2::OpenGL;

std::map<void*, Context*> Context::_contextList;

// ------------------------------------------------------------------------------------------------
Context::Context()
: _hDC( nullptr )
, _hRC( nullptr )
{}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	release();
}

// ------------------------------------------------------------------------------------------------
int Context::id() const
{
	return (int)( _hRC );
}

// ------------------------------------------------------------------------------------------------
Context* Context::create( int majorVersion, int minorVersion )
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	auto HWND = GetActiveWindow();
	auto DC = GetDC( HWND );
	if( !DC )
		return nullptr;

	{
		PIXELFORMATDESCRIPTOR pfd;
		memset( &pfd, 0, sizeof( pfd ) );

		pfd.nSize	    = sizeof( PIXELFORMATDESCRIPTOR );
		pfd.nVersion	= 1;
		pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType	= PFD_TYPE_RGBA;
		pfd.dwLayerMask	= PFD_MAIN_PLANE;
		pfd.cColorBits  = 32;
		pfd.cDepthBits  = 24;

		auto pxlfmt = ChoosePixelFormat( DC, &pfd );
		if( !pxlfmt || !SetPixelFormat( DC, pxlfmt, &pfd ) )
			return nullptr;
	}

	auto RC = wglCreateContext( DC );
	if( !RC )
	{
		ReleaseDC( HWND, DC );
		return nullptr;
	}

	Context *c = new Context;
	c->makeCurrent();
	c->initExtensions();

	_contextList.insert( std::make_pair( c->_hRC, c ) );

	return c;
#else
	return nullptr;
#endif
}

// ------------------------------------------------------------------------------------------------
Context* Context::getCurrent()
{
	void* context; // _hRC
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	context = wglGetCurrentContext();
#else
	context = glXGetCurrentContext();
#endif
	auto it = _contextList.find( context );

	// context created externally
	if( it == _contextList.end() )
	{
		// make an entry
		Context *c = new Context;
		c->_hRC = context;
		c->_hDC  = wglGetCurrentDC();
		
		_contextList.insert( std::make_pair( c->_hRC, c ) );
		return c;
	}

	return it->second;
}

// ------------------------------------------------------------------------------------------------
void Context::release()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	if( _hRC )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( (HGLRC) _hRC );
		_hRC = NULL;
	}

	if( _hDC && !ReleaseDC( NULL, (HDC) _hDC ) ) // HWND should be passed. to be tested.
		_hDC = NULL;
#else
#endif
}

// ------------------------------------------------------------------------------------------------
bool Context::initExtensions() const
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
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	if( _hDC && _hRC )
		wglMakeCurrent( (HDC) _hDC, (HGLRC) _hRC );
#else
#endif
}

// ------------------------------------------------------------------------------------------------
void Context::swapBuffers()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	if( _hDC )
		SwapBuffers( (HDC) _hDC );
#else
#endif
}

// ------------------------------------------------------------------------------------------------
void Context::enableVSync( bool enabled )
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	//if( glewIsSupported("wglSwapIntervalEXT") )
	   // wglSwapIntervalEXT(enabled ? 1 : 0);
#else
#endif
}