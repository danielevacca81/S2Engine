// OpenGLObject.cpp
//
#include "OpenGLObject.h"

#include "OpenGL.h" 
#include "OpenGLWrap.h"
#include "VertexArray.h"
#include "FrameBuffer.h"

using namespace Renderer;

//#define ASSERT_ON_CHECKGLCONTEXT
#define ALLOW_CONTEXT_RESOURCE_SHARING

// ------------------------------------------------------------------------------------------------
static void myAssert( bool condition )
{
	#ifdef ASSERT_ON_CHECKGLCONTEXT
		assert( condition );
	#else
		if( !condition )
			int gg = 0;
	#endif
}

// ------------------------------------------------------------------------------------------------
void OpenGLObject::setLabel( const std::string &label )
{
#ifdef _DEBUG
	// todo  bisognerebbe verificare la disponibilita' di glObjectLabel.
	// glObjectLabel e' disponibile dalla versione 4.3 di OpenGL o se' supportata 
	// l'estensione GL_KHR_debug. 

	_label = label;

	const auto labelIdentifier = objectLabelIdentifier();
	if( labelIdentifier == 0 || _objectID == 0 )
		return;

	bind();
	glObjectLabel( labelIdentifier, _objectID, -1, _label.c_str() );
	glCheck;
	unbind();
#endif
}

// ------------------------------------------------------------------------------------------------
std::string OpenGLObject::label() const
{
#ifdef _DEBUG
	return _label;
#else
	return {};
#endif
}

// ------------------------------------------------------------------------------------------------
void OpenGLObject::storeGlContext() 
{
#if defined( _DEBUG ) && defined( _WIN32 )
	_deviceContext     = wglGetCurrentDC();
	_glRenderingContex = wglGetCurrentContext();
	myAssert( _glRenderingContex != 0 );
#endif
}

// ------------------------------------------------------------------------------------------------
void OpenGLObject::checkGlContext() const
{
#if defined( _DEBUG ) && defined( _WIN32 )

	auto deviceContext     = wglGetCurrentDC();
	auto glRenderingContex = wglGetCurrentContext();

	myAssert( glRenderingContex );

	#ifdef ALLOW_CONTEXT_RESOURCE_SHARING
		if( dynamic_cast<const VertexArray*>( this ) || dynamic_cast<const FrameBuffer*>( this ) )
			myAssert( glRenderingContex == _glRenderingContex );
	#else
		myAssert( glRenderingContex == _glRenderingContex );
	#endif
		
#endif
}


