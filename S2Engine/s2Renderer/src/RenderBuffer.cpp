// RenderBuffer.cpp
//
#include "RenderBuffer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

using namespace Renderer;

// ------------------------------------------------------------------------------------------------
RenderBufferPtr RenderBuffer::New( const Format &format, int width, int height, int samples )
{
	return std::make_shared<RenderBuffer>( format, width, height, samples );
}

// ------------------------------------------------------------------------------------------------
//RenderBuffer::RenderBuffer()
//: RenderBuffer( Format::RGBA8, 0, 0, 0 )
//{}

// ------------------------------------------------------------------------------------------------
RenderBuffer::RenderBuffer( const Format &format, int width, int height, int samples )
: _format( format )
, _width( width )
, _height( height )
, _samples( samples )
{
	create();
}

// ------------------------------------------------------------------------------------------------
//RenderBuffer::RenderBuffer( RenderBuffer &&other )
//: RenderBuffer()
//{
//	std::swap( _format , other._format  );
//	std::swap( _width  , other._width   );
//	std::swap( _height , other._height  );
//	std::swap( _samples, other._samples );
//
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//}

// ------------------------------------------------------------------------------------------------
RenderBuffer::~RenderBuffer()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
void RenderBuffer::bind() const
{
	checkGlContext();
	glBindRenderbuffer( GL_RENDERBUFFER, _objectID );
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void RenderBuffer::unbind() const
{
	checkGlContext();
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	glCheck;
}

// -------------------------------------------------------------------------------------------------
void RenderBuffer::reset()
{
	OpenGLObject::reset();

	_format  = Format::RGBA8;
	_width   = 0;
	_height  = 0;
	_samples = 0;
}

// ------------------------------------------------------------------------------------------------
bool RenderBuffer::create()
{
	destroy();

	storeGlContext();
	glGenRenderbuffers( 1, &_objectID );
	bind();

	if( _samples == 0 )  glRenderbufferStorage( GL_RENDERBUFFER, glWrap(_format), _width, _height );	
	else                 glRenderbufferStorageMultisample( GL_RENDERBUFFER, _samples, glWrap(_format), _width, _height );

	glCheck;
	unbind();

	_created = true;
	return _created;
}

// ------------------------------------------------------------------------------------------------
void RenderBuffer::destroy()
{
	if( !isCreated() )
		return;
	
	checkGlContext();
	glDeleteRenderbuffers( 1, &_objectID );
	glCheck;

	reset();
}


// render buffers shall not be directly accessible. only use through a fbo
//// ------------------------------------------------------------------------------------------------
//void RenderBuffer::bind() const
//{
//	glBindRenderbuffer( GL_RENDERBUFFER, _objectID );
//}

// ------------------------------------------------------------------------------------------------
//unsigned int RenderBuffer::id() const
//{
//	return _objectID;
//}