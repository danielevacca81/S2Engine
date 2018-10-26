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

std::map<void*, std::weak_ptr<Context> > Context::_contextList;

// ------------------------------------------------------------------------------------------------
Context::Context()
: _hDC( nullptr )
, _hRC( nullptr )
, _external( false )
{}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	//releaseResources();

	if( !_external )
		release();
}

// ------------------------------------------------------------------------------------------------
int64_t Context::id() const
{
	return (int64_t)( _hRC );
}

// ------------------------------------------------------------------------------------------------
ContextPtr Context::New( int majorVersion, int minorVersion )
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

	ContextPtr c = std::make_shared<Context>();
	c->makeCurrent();
	c->initExtensions();

	_contextList.insert( std::make_pair( c->_hRC, c ) );

	return c;
#else
	return nullptr;
#endif
}

// ------------------------------------------------------------------------------------------------
ContextPtr Context::Current()
{
	void* hRC; // _hRC
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	hRC = wglGetCurrentContext();
#else
	hRC = glXGetCurrentContext();
#endif
	auto it = _contextList.find( hRC );

	// context created externally
	if( it == _contextList.end() )
	{
		// make an entry
		ContextPtr c = std::make_shared<Context>();
		c->_hRC      = hRC;
		c->_hDC      = wglGetCurrentDC();
		c->_external = true;
		
		c->makeCurrent();
		c->initExtensions();
		
		_contextList.insert( std::make_pair( c->_hRC, c ) );
		return c;
	}

	return it->second.lock();
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

// ------------------------------------------------------------------------------------------------
//void Context::addResource( const ResourcePtr &res )
//{
//	_resources.insert( res );
//
//	std::cout << "Added resource [" << res->name() << "] to context " << id() << std::endl;
//}
//
//// ------------------------------------------------------------------------------------------------
//void Context::releaseResources()
//{
//	if( _resources.empty() )
//	{
//		std::cout << "Releasing resources from context " << id() << ":" << "No resources" << std::endl;
//		return;
//	}
//
//	std::cout << "Releasing resources from context " << id() << ":" << std::endl;	
//	for( auto &r : _resources )
//		std::cout << "    " << r->name();
//	
//	std::cout << std::endl;	
//
//	_resources.clear();
//}