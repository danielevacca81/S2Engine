// RenderBuffer.cpp
//
#include "RenderBuffer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

using namespace s2::OpenGL;


// ------------------------------------------------------------------------------------------------
RenderBuffer::RenderBuffer( const Format &format, int width, int height, int samples )
{
	glGenRenderbuffers( 1, &_bufferID );
	glBindRenderbuffer( GL_RENDERBUFFER, _bufferID );
	
	if( samples == 0 )  glRenderbufferStorage( GL_RENDERBUFFER, glWrap(format), width, height );	
	else                glRenderbufferStorageMultisample( GL_RENDERBUFFER, samples, glWrap(format), width, height );

	glCheck;
}

// ------------------------------------------------------------------------------------------------
RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers( 1, &_bufferID );
	glCheck;
}

// ------------------------------------------------------------------------------------------------
unsigned int RenderBuffer::id() const
{
	return _bufferID;
}