// FrameBuffer.cpp
//
#include "FrameBuffer.h"

#include "OpenGL.h" 
#include "OpenGLWrap.h"
#include "Renderer.h"

#include <algorithm>
#include <iostream>

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#ifdef DrawState
#undef DrawState
#endif
#endif


namespace s2 {
namespace OpenGL {

std::map<void*,FrameBufferPtr> FrameBuffer::Default;
FrameBufferPtr FrameBuffer::Current;

// ------------------------------------------------------------------------------------------------
FrameBufferPtr FrameBuffer::New( bool default )
{
	return std::make_shared<FrameBuffer>( default );
}

// ------------------------------------------------------------------------------------------------
FrameBufferPtr FrameBuffer::getDefault()
{
	void* context;
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
	context = wglGetCurrentContext();
#else
	context = glXGetCurrentContext();
#endif

	const auto i = Default.find( context );

	// already created
	if( i != Default.end() )
		return i->second;

	FrameBufferPtr fb = FrameBuffer::New( true );
	Default.insert( std::make_pair( context, fb ) );
	return fb;
}

// ------------------------------------------------------------------------------------------------
FrameBuffer::FrameBuffer( bool default )
: _fboID( 0 )
, _attachmentsChanged( false )
, _parametersChanged( false )
, _buffersChanged( false )
{
	if( default )
		_fboID = 0;
	else
		glGenFramebuffers( 1, &_fboID );

	glCheck;

	_readBuffer  = ColorAttachment0;
	_drawBuffers = { ColorAttachment0 };
}

// ------------------------------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
	if( Current->_fboID == this->_fboID )
		Current = nullptr;

	if( _fboID != 0 )
		glDeleteFramebuffers( 1, &_fboID );

	glCheck;
}

// ------------------------------------------------------------------------------------------------
std::string FrameBuffer::info() const
{
	std::string ret( "FBO Failed: Unknown Error" );

	const GLenum r = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	switch( r )
	{
	case GL_FRAMEBUFFER_COMPLETE:                       ret = "FBO Ok";                        break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			ret = "FBO Failed: Attachment Error";  break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	ret = "FBO Failed: Missing attachment"; break;
	//case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:	    	ret = "FBO Failed: Dimensions Error";  break;
	//case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:			    ret = "FBO Failed: Formats Error";     break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			ret = "FBO Failed: Incomplete Draw buffer"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			ret = "FBO Failed: Incomplete Read buffer"; break;
	case GL_FRAMEBUFFER_UNSUPPORTED:					ret = "FBO Failed: Not Supported";     break;
	case GL_FRAMEBUFFER_UNDEFINED:					    ret = "FBO Failed: Undefined";         break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			ret = "FBO Failed: Incomplete Multisample";     break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:       ret = "FBO Failed: Incomplete Layer targets";   break;
	}

	return ret;
}

// ------------------------------------------------------------------------------------------------
bool FrameBuffer::checkAttatchments() const
{
	const GLenum r = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	return r == GL_FRAMEBUFFER_COMPLETE;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::set( ) const
{
	//// Specify what to render an start acquiring
	//std::vector<GLenum> buffers;
	//for( size_t i=0; i < _attachments.size(); ++i )
	//	if( glWrap( _attachments[i].attachmentName ) != GL_DEPTH_ATTACHMENT )
	//		buffers.push_back( glWrap( _attachments[i].attachmentName ) );

	//if( buffers.empty() )
	//	return;

	glBindFramebuffer( GL_FRAMEBUFFER, _fboID );
	setAttachments();
	if( !checkAttatchments() )
	{
		std::cout << info() << std::endl;
		return;
	}


	// setDrawBuffers
	//{
	//	if( _fboID != 0 && readDrawChanged )
	//	{
	//		glReadBuffer( getBuffer( readBuffer ) );
	//		if( drawBufferCount == 1 )
	//		{
	//			glDrawBuffer( getBuffer( drawBuffers[0] ) );
	//		}
	//		else
	//		{
	//			GLenum drawBufs[4];
	//			for( int i = 0; i < drawBufferCount; ++i )
	//			{
	//				drawBufs[i] = getBuffer( drawBuffers[i] );
	//			}
	//			glDrawBuffers( drawBufferCount, drawBufs );
	//		}
	//		readDrawChanged = false;
	//		assert( getError() == 0 );
	//	}
	//}

	setParameters();
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::setAttachments() const
{
	if( !_attachmentsChanged )
		return;

	for( auto &att : _attachments )
	{
        //if (textures[i] == NULL) {
        //    glFramebufferRenderbuffer(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_RENDERBUFFER, 0);
        //    continue;
        //}

		if( att.isRenderBuffer )
		{
			glFramebufferRenderbuffer( GL_FRAMEBUFFER, att.attachmentName, GL_RENDERBUFFER, att.attachmentID );
		}
		else
		{
			switch( att.attachmentType )
			{
			case GL_TEXTURE_1D:	glFramebufferTexture1D( GL_FRAMEBUFFER, att.attachmentName, att.attachmentType, att.attachmentID, 0 ); break;
			case GL_TEXTURE_2D:	glFramebufferTexture2D( GL_FRAMEBUFFER, att.attachmentName, att.attachmentType, att.attachmentID, 0 ); break;

				// @TODO:
				//case Texture3D::target():	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+_numAttachments, texture->target(), texture->id(), 0); break;
			}
			/*
			int id = textures[i].cast<Texture>()->getId();
			if (textures[i].cast<Texture1DArray>() != NULL ||
				textures[i].cast<Texture2DArray>() != NULL ||
				textures[i].cast<Texture2DMultisampleArray>() != NULL ||
				textures[i].cast<Texture3D>() != NULL)
			{
				if (layers[i] == -1) {
					glFramebufferTexture(GL_FRAMEBUFFER, ATTACHMENTS[i], id, levels[i]);
				} else {
					glFramebufferTextureLayer(GL_FRAMEBUFFER, ATTACHMENTS[i], id, levels[i], layers[i]);
				}
			} else if (textures[i].cast<Texture1D>() != NULL) {
				glFramebufferTexture1D(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_TEXTURE_1D, id, levels[i]);
			} else if (textures[i].cast<Texture2D>() != NULL) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_TEXTURE_2D, id, levels[i]);
			} else if (textures[i].cast<TextureRectangle>() != NULL) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_TEXTURE_RECTANGLE, id, levels[i]);
			} else if (textures[i].cast<Texture2DMultisample>() != NULL) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_TEXTURE_2D_MULTISAMPLE, id, levels[i]);
			} else if (textures[i].cast<TextureCube>() != NULL) {
				if (layers[i] == -1) {
					glFramebufferTexture(GL_FRAMEBUFFER, ATTACHMENTS[i], id, levels[i]);
				} else {
					glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENTS[i], getCubeFace((CubeFace) layers[i]), id, levels[i]);
				}
			} else if (textures[i].cast<TextureCubeArray>() != NULL) {
				glFramebufferTextureLayer(GL_FRAMEBUFFER, ATTACHMENTS[i], id, levels[i], layers[i]);
			}*/
		}
        glCheck;
    }

	_attachmentsChanged = false;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::setParameters() const
{
	if( _parametersChanged )
		glViewport( _viewport.left(), _viewport.bottom(), _viewport.width(), _viewport.height() );

	_parametersChanged = false;
}


// ------------------------------------------------------------------------------------------------
void FrameBuffer::setViewport( const Math::Rectangle &viewport )
{
	_viewport = viewport;
	_parametersChanged = true;
}

// ------------------------------------------------------------------------------------------------
Math::Rectangle FrameBuffer::viewport() const
{
	return _viewport;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::clear( const ClearState &cs )
{
	_stateManager.applyClearState( cs );

	glCheck;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::draw( const PrimitiveType &primitive, const Mesh &mesh, const DrawState &ds )
{
	set();

	_stateManager.applyDrawState( ds );
	// applyView?

	OpenGL::Renderer r;
	r.draw( primitive,mesh );
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::draw( const PrimitiveType &primitive, const VertexArray &va, const DrawState &ds )
{
	set();

	_stateManager.applyDrawState( ds );
	// applyView?

	OpenGL::Renderer r;
	r.draw( primitive,va );
}

// ------------------------------------------------------------------------------------------------
/*void FrameBuffer::unBindAttachment()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}*/

// ------------------------------------------------------------------------------------------------
/*void FrameBuffer::readColorAttachment(unsigned int attachPoint, GLenum format, GLenum type, void *pixels)
{
	if( attachPoint > _numAttachments )
		return;

	glPushAttrib( GL_ENABLE_BIT );
	glEnable( _attachments[attachPoint].attachmentType );
	glBindTexture( _attachments[attachPoint].attachmentType, _attachments[attachPoint].attachmentID);
	glGetTexImage( _attachments[attachPoint].attachmentType, 0, format, type, pixels);
	glBindTexture( _attachments[attachPoint].attachmentType, 0);
	glPopAttrib();
}*/

// ------------------------------------------------------------------------------------------------
/*void FrameBuffer::readDepthAttachment( float *pixels )
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
	glReadPixels(0, 0, _width, _height, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
*/

// ------------------------------------------------------------------------------------------------
//bool FrameBuffer::addRenderBufferAttachment( GLuint targetID, GLenum format )
//{
//	if( !isValid() )
//		return false;
//
//	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
//
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+_numAttachments,GL_RENDERBUFFER, targetID);
//
//	//GLenum *drawBuffers = new GLenum[numAttachments+1];
//	//for(int i=0;i<numAttachments+1; ++i)
//	//{
//	//	drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
//	//}
//
//	//glDrawBuffers(numAttachments+1, drawBuffers);
//	//delete [] drawBuffers;
//	// check for any error
//	const bool statusOk = checkStatus();
//
//	// Bind no FBO
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		
//
//	if( statusOk )
//	{
//		_attachments[_numAttachments].attachmentName = format;
//			
//		_numAttachments++;
//	}
//	
//	return statusOk;
//}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::attachTextureTarget( const AttachmentPoint &attachPoint, const TexturePtr &texture )
{
	Attachment a = { attachPoint, glWrap( texture->type() ) , texture->id(),false };
	
	auto it = std::remove_if( 
		_attachments.begin(), _attachments.end(),
		[a] ( const s2::OpenGL::FrameBuffer::Attachment &i )
		{
			return a.attachmentName == i.attachmentName;
		}
	);
	
	_attachments.push_back( a );
	_attachmentsChanged = true;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::attachRenderTarget( const AttachmentPoint &attachPoint, const RenderBufferPtr &renderBuffer )
{
	Attachment a = { attachPoint, 0, renderBuffer->id(),true };
	_attachments.push_back( a );
	_attachmentsChanged = true;

	//glBindFramebuffer( GL_FRAMEBUFFER, _fboID );

	//glGenRenderbuffers( 1, &_depthBuffer );
	//glBindRenderbuffer( GL_RENDERBUFFER, _depthBuffer );
	//glRenderbufferStorage( GL_RENDERBUFFER, glWrap( depthFormat ), width, height );
	//glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer );

	//const bool statusOk = checkStatus();

	//glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	//if( !statusOk )
	//{
	//	glDeleteRenderbuffers( 1, &_depthBuffer );
	//	return false;
	//}

	//_depthFormat = depthFormat;
	//return true;
}

// ------------------------------------------------------------------------------------------------
//void FrameBuffer::removeAttachment()
//{
//	_numAttachments--;
//	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+_numAttachments, _attachments[_numAttachments].attachmentType, 0, 0);
//		
//	//GLenum *drawBuffers = new GLenum[numAttachments+1];
//	//for(int i=0;i<numAttachments+1; ++i)
//	//{
//	//	drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
//	//}
//
//	//glDrawBuffers(numAttachments+1, drawBuffers);
//	//delete [] drawBuffers;
//}

// ------------------------------------------------------------------------------------------------
//void FrameBuffer::setDrawBuffers( unsigned int num, GLenum buffers[] )
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
//		
//	if( num == 0 )	glDrawBuffer( GL_NONE );
//	else			glDrawBuffers( num, buffers );
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
//}

// ------------------------------------------------------------------------------------------------
//void FrameBuffer::setReadBuffer(GLenum buffer)
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
//	glReadBuffer(buffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
}

}