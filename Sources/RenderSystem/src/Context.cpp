// Context.cpp
//
#include "Context.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "Surface.h"

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <GL/glx.h>
#endif

#include <map>
#include <iostream>

using namespace RenderSystem;

// mutex??
static std::map<uint64_t, Context*> gRegistry;
//Context* Context::gCurrent = nullptr;


// ------------------------------------------------------------------------------------------------
Context *Context::current()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	uint64_t handle = (uint64_t) wglGetCurrentContext();
#else
	uint32_t handle = glXGetCurrentContext();
#endif

	//  @todo: mutex?
	//		   also: avoid find in getCurrent and just return current context pointer
	Context* c = nullptr;
	auto found = gRegistry.find(handle);
	if( found == gRegistry.end() )
	{
		c = new Context;
		gRegistry.emplace( std::make_pair( handle, c ) );
	}
	else
	{
		c = found->second;
	}

	return c;
}

// ------------------------------------------------------------------------------------------------
//Context* Context::current()
//{
//	return gCurrent;
//}

// ------------------------------------------------------------------------------------------------
Context::Context()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	_nativeHandle = (uint64_t) wglGetCurrentContext();
#else
	_nativeHandle = glXGetCurrentContext();
#endif
	if( !glewInit() == GLEW_OK )
		throw std::runtime_error( "GLEW initialization failed!" );

	glGetIntegerv( GL_FRAMEBUFFER_BINDING, &_defaultFBO );

	_info.init();

	std::cout
		<< "Registering Context:" << std::hex << (uint32_t) _nativeHandle << '\n'
		<< _info.toString()
		<< std::endl
		;
}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	//if( current() == this )
	//	gCurrent = nullptr;
	std::cout << "Contexts: "<< '\n';
	for( auto& i : gRegistry )
		std::cout << "   handle " << std::hex << (uint32_t) i.first << " ContextPtr " <<  i.second << '\n';
	
	std::cout << "Destroying context: " << std::hex << (uint32_t) _nativeHandle << '\n';
	gRegistry.erase( _nativeHandle );

	std::cout << "Contexts: " << '\n';
	for( auto& i : gRegistry )
		std::cout << "   handle " << std::hex << (uint32_t) i.first << " ContextPtr " << i.second << '\n';
}

// ------------------------------------------------------------------------------------------------
Surface* Context::createSurface( const std::initializer_list<Surface::AttachmentDescription> &init ) { return new Surface( init ); }

// ------------------------------------------------------------------------------------------------
void Context::beginRendering() { _stateManager.disableShadowingOneShot(); }
void Context::endRendering()   { glFlush(); }





#if 0
#include "OpenGL.h"
#include "Extensions.h"
#include "ContextRegistry.h"
#include "OpenGLWrap.h"

#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <GL/glx.h>
#endif

using namespace RenderSystem;

#if 0

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
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		c->_hDC = wglGetCurrentDC();
#else
#endif
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
	return Renderer::initExtensions();
}

// ------------------------------------------------------------------------------------------------
std::vector<std::string> Context::extensions() const
{
	return Renderer::extensions();
}

// ------------------------------------------------------------------------------------------------
std::string Context::info() const
{
	return Renderer::contextInfo();
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

#endif


// ------------------------------------------------------------------------------------------------
Context::NativeHandle Context::NativeHandle::current()
{
	NativeHandle retval;
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	retval._deviceContext     = wglGetCurrentDC();
	retval._glRenderingContex = wglGetCurrentContext();
	return retval;
#else
	retval._glRenderingContex = glXGetCurrentContext();
#endif
}

// ------------------------------------------------------------------------------------------------
Context::NativeHandle Context::NativeHandle::null()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	return {};
#else
	return {};
#endif
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
/*Context::RawGLObject::~RawGLObject()
{
	if( objectID != 0 )
		switch( type )
		{
		default:
		case RawGLObject::Type::None:
			return;
		}
}*/


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Context::Context( const NativeHandle &nativeHandle )
:  _nativeHandle( nativeHandle )
{
}

// ------------------------------------------------------------------------------------------------
Context::~Context()
{
	std::cout << "Renderer::~Context " << _name << "\n";

	ContextRegistry::registry()->onDestroyContext( this );
	// \warning gestire distruzione di _nativeHandle nelle classi derivate se occorre
}

// ------------------------------------------------------------------------------------------------
bool Context::isCurrent() const
{
	return this == ContextRegistry::currentContext();
}

// ------------------------------------------------------------------------------------------------
void Context::makeCurrent()
{
	ContextRegistry::registry()->makeCurrent( this );
}

// ------------------------------------------------------------------------------------------------
void Context::clear( const ClearState& cs, const FrameBufferPtr& fb )
{
	assert( fb->context().get() == this );
	assert( isCurrent() );

	fb->bind();
	_stateManager.setClearState( cs );
}

// ------------------------------------------------------------------------------------------------
void Context::draw( const PrimitiveType& primitiveType, const VertexArrayPtr& va,
					const DrawState& ds, const FrameBufferPtr& fb )
{
	assert( fb->context().get() == this );
	assert( isCurrent() );

	if( !va )
		return;

	fb->bind();
	_stateManager.setDrawState( ds );

	draw( primitiveType, va );
}

// ------------------------------------------------------------------------------------------------
void Context::draw( const PrimitiveBatch& batch, const DrawState& ds, const FrameBufferPtr& fb )
{
	assert( fb->context().get() == this );
	assert( isCurrent() );


	auto pBuffer = PrimitiveBuffer::New();
	pBuffer->setVertices( batch._batch.vertices );
	pBuffer->setTextureCoords( batch._batch.textureCoords );
	pBuffer->setColors( batch._batch.colors );
	pBuffer->setNormals( batch._batch.normals );
	pBuffer->setIndices( batch._batch.indices );


	auto ds2 = ds;
	ds2.renderState.primitiveRestart.enabled = true;
	ds2.renderState.primitiveRestart.index = batch._primitiveRestartIndex;


	fb->bind();
	_stateManager.setDrawState( ds );
	
	draw( batch._batch._primitiveType, pBuffer, ds2, fb );
}


// ------------------------------------------------------------------------------------------------
void Context::draw( const PrimitiveType& primitiveType, const PrimitiveBufferPtr& p,
					const DrawState& ds, const FrameBufferPtr& fb )
{
	assert( fb->context().get() == this );
	assert( isCurrent() );

	if( !p )
		return;

	fb->bind();
	_stateManager.setDrawState( ds );

	draw( primitiveType, p->_vao );
}

//-------------------------------------------------------------------------------------------------
inline void Context::draw( const PrimitiveType &primitive, const VertexArrayPtr &va )
{
	va->bind();
	
	if( va->isIndexed() )
	{		
		glDrawRangeElements( glWrap(primitive),
			                 0,
							 va->maxArrayIndex(),
							 va->indexBuffer().count(),
			                 glWrap( va->indexBuffer().dataType() ),
							 BUFFER_OFFSET(0) );
		glCheck;
	}
	else
	{
		glDrawArrays( glWrap(primitive), 0, va->maxArrayIndex() + 1);
		glCheck;
	}
}

// ------------------------------------------------------------------------------------------------
void Context::draw( const PrimitiveType& primitiveType, const PrimitiveBufferPtr& p,
					const DrawState& ds, const unsigned int targetFBO )
{
	assert( isCurrent() );

	if( !p )
		return;

	glBindFramebuffer( GL_FRAMEBUFFER, targetFBO );
	glCheck;

	_stateManager.setDrawState( ds );

	draw( primitiveType, p->_vao );
}
#endif