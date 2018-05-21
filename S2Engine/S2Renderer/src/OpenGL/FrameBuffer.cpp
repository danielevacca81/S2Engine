// FrameBuffer.cpp
//
#include "FrameBuffer.h"

#include "OpenGL.h" 
#include "OpenGLWrap.h"

namespace OpenGL{

// ------------------------------------------------------------------------------------------------
FrameBuffer::FrameBuffer()
: _width(0)
, _height(0)
, _fbo(0)
, _depthFormat(DepthNone)
, _bound(false)
{
}

// ------------------------------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::destroy()
{
	if( !isValid() ) return;

	glDeleteFramebuffers(1, &_fbo);

	if( glWrap(_depthFormat) != GL_NONE )
		glDeleteRenderbuffers( 1, &_depthBuffer );
}

// ------------------------------------------------------------------------------------------------
std::string FrameBuffer::info() const
{
	std::string ret("FBO Failed: Unknown Error");

	const GLenum r = glCheckFramebufferStatus( GL_FRAMEBUFFER_EXT );
	switch( r )
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:                       ret = "FBO Ok";                        break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:			ret = "FBO Failed: Attachment Error";  break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:	ret = "FBO Failed: Missing attachment";break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:	    	ret = "FBO Failed: Dimensions Error";  break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:			    ret = "FBO Failed: Formats Error";     break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:			ret = "FBO Failed: Draw buffer Error"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:			ret = "FBO Failed: Read buffer Error"; break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:					ret = "FBO Failed: Not Supported";     break;
	}

	return ret;
}

// ------------------------------------------------------------------------------------------------
bool FrameBuffer::checkStatus() const
{
	const GLenum r = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
	return r == GL_FRAMEBUFFER_COMPLETE_EXT;
}

// ------------------------------------------------------------------------------------------------
bool FrameBuffer::create()
{
	destroy();
	
	glGenFramebuffersEXT(1, &_fbo);
	return true;
}

// ------------------------------------------------------------------------------------------------
// create a FBO with a new texture as attachment
/*bool FrameBuffer::create( unsigned int w, unsigned int h, GLenum colorformat, GLenum depthformat, bool generateMipmap )
{
	destroy(true);

	// invalid parameters
	if( w == 0 || h == 0 )
		return false;

	// Create empty color texture ?
	if( colorformat != GL_NONE )
	{
		_attachments[0].attachmentType   = GL_TEXTURE_2D;
		_attachments[0].attachmentFormat = GL_RGBA;

		glGenTextures(1,&_attachments[0].attachmentID );
		glBindTexture(   _attachments[0].attachmentType, _attachments[0].attachmentID );
		glTexParameterf( _attachments[0].attachmentType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( _attachments[0].attachmentType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		if( generateMipmap )
		{
			glTexParameteri(  _attachments[0].attachmentType, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri(  _attachments[0].attachmentType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glGenerateMipmap( _attachments[0].attachmentType );
		}
		else
		{
			glTexParameterf( _attachments[0].attachmentType, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameterf( _attachments[0].attachmentType, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		}
		glTexImage2D(  _attachments[0].attachmentType, 0, _attachments[0].attachmentFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
		glBindTexture( _attachments[0].attachmentType, 0 );
			
		_numAttachments = 1;
	}

	// Want a depth attachment?
	if( depthformat != GL_NONE )
	{
		glGenTextures( 1, &_depthBuffer );
		glBindTexture( GL_TEXTURE_2D, _depthBuffer );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
		glTexImage2D( GL_TEXTURE_2D, 0, depthformat, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	// Create the frame buffer
	glGenFramebuffersEXT(1, &_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);
	//glCheck;

	// attach color
	if( _numAttachments == 1 )
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, _attachments[0].attachmentType, _attachments[0].attachmentID, 0);
	//glCheck;

	// attach depth
	if( _depthBuffer != 0 )
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0);
	//glCheck;

	// check for any error
	const bool statusOk = checkStatus();

	// Bind no FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if( !statusOk )
	{
		destroy(true);
		return false;
	}

	_width		= w;
	_height		= h;
	_depthFormat = depthformat;

	return true;
}*/

// ------------------------------------------------------------------------------------------------
bool   FrameBuffer::isValid()     const  { return _fbo != 0;   }
bool   FrameBuffer::isBound()     const  { return _bound;      }

// ------------------------------------------------------------------------------------------------
/*void FrameBuffer::bindColorAttachment( unsigned int i )	
{
	if( i<_numAttachments ) 
		glBindTexture( _attachments[i].attachmentType, _attachments[i].attachmentID );
}*/

// ------------------------------------------------------------------------------------------------
/*void FrameBuffer::bindDepthAttachment()
{
	glBindTexture(GL_TEXTURE_2D, _depthBuffer);
}*/

// ------------------------------------------------------------------------------------------------
void FrameBuffer::bind( int width, int height )
{
	if( !isValid() )
		return;

	// Specify what to render an start acquiring
	std::vector<GLenum> buffers;
	for( size_t i=0; i<_attachments.size(); ++i )
		if( glWrap(_attachments[i].attachmentName) != GL_DEPTH_ATTACHMENT )
			buffers.push_back( glWrap(_attachments[i].attachmentName) );

	if( buffers.empty() )
		return;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, width, height);
	glDrawBuffers( buffers.size(), &buffers[0]);

	_bound = glValidate;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::clear( float r, float g, float b, float a )
{
	if( isBound() )
	{
		glClearColor( r,g,b,a );
		glClear(GL_COLOR_BUFFER_BIT | (glWrap(_depthFormat) != GL_NONE ? GL_DEPTH_BUFFER_BIT : 0) );	
	}
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::unBind()
{
	if( !isBound() )
		return;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 	
	glPopAttrib();
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
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, _fbo);
	glReadPixels(0, 0, _width, _height, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}
*/

// ------------------------------------------------------------------------------------------------
//bool FrameBuffer::addRenderBufferAttachment( GLuint targetID, GLenum format )
//{
//	if( !isValid() )
//		return false;
//
//	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
//
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT+_numAttachments,GL_RENDERBUFFER, targetID);
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
bool FrameBuffer::attachTextureTarget( const TexturePtr &texture, AttachmentPoint attachPoint )
{
	if( !isValid() )
		return false;

	//if( _attachments.empty() )
	//{
	//	_width  = texture->width();
	//	_height = texture->height();
	//}
	//else
	//{
	//	// incompatible size
	//	if( _width != texture->width() || _height != texture->height() )
	//		return false;
	//}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);

	switch( texture->type() )
	{
	case Texture::Texture_1D:	glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, attachPoint, GL_TEXTURE_1D, texture->id(), 0); break;
	case Texture::Texture_2D:	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachPoint, GL_TEXTURE_2D, texture->id(), 0); break;
	
	// @TODO:
	//case Texture3D::target():	glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0+_numAttachments, texture->target(), texture->id(), 0); break;
	}
	
	//GLenum *drawBuffers = new GLenum[numAttachments+1];
	//for(int i=0;i<numAttachments+1; ++i)
	//{
	//	drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
	//}

	//glDrawBuffers(numAttachments+1, drawBuffers);
	//delete [] drawBuffers;
	// check for any error
	const bool statusOk = checkStatus();

	// Bind no FBO
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);


	if( !statusOk )
		return false;

	Attachment a = { attachPoint, texture->type(), texture->id() };
	_attachments.push_back( a );

	return true;
}

// ------------------------------------------------------------------------------------------------
bool FrameBuffer::attachDepthRenderTarget( DepthFormat depthFormat, int width, int height )
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);

	glGenRenderbuffersEXT(1, &_depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, glWrap(depthFormat), width, height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, _depthBuffer);
	
	const bool statusOk = checkStatus();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	if( !statusOk )
	{
		glDeleteRenderbuffers( 1, &_depthBuffer );
		return false;
	}
	
	_depthFormat = depthFormat;
	return true;
}

// ------------------------------------------------------------------------------------------------
//void FrameBuffer::removeAttachment()
//{
//	_numAttachments--;
//	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+_numAttachments, _attachments[_numAttachments].attachmentType, 0, 0);
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
//	glBindFramebuffer(GL_FRAMEBUFFER_EXT, _fbo);
//		
//	if( num == 0 )	glDrawBuffer( GL_NONE );
//	else			glDrawBuffers( num, buffers );
//
//	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);	
//}

// ------------------------------------------------------------------------------------------------
//void FrameBuffer::setReadBuffer(GLenum buffer)
//{
//	glBindFramebuffer(GL_FRAMEBUFFER_EXT, _fbo);
//	glReadBuffer(buffer);
//	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
//}
}
