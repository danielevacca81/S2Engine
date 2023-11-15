// FrameBuffer.cpp
//
#include "FrameBuffer.h"

#include "OpenGL.h" 
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"

#include <algorithm>


using namespace RenderSystem;

// ------------------------------------------------------------------------------------------------
static void makeGLAttachment( const FrameBuffer::AttachmentPoint &attachPoint, const Texture2DPtr &texture )
{
	if( texture && texture->isCreated() ) glFramebufferTexture( GL_FRAMEBUFFER, glWrap( attachPoint ), texture->id(), 0 );
	else                                  glFramebufferTexture( GL_FRAMEBUFFER, glWrap( attachPoint ), 0, 0 );

	glCheck;
}
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
FrameBufferPtr FrameBuffer::New()
{
	return std::make_shared<FrameBuffer>();
}

// ------------------------------------------------------------------------------------------------
FrameBuffer::FrameBuffer()
: //_context( ContextRegistry::currentContextWeakPtr() )
	_changes( Changes::None )
, _colorAttachments( kMaxColorAttachment )
, _colorAttachmentCount( 0 )
{
	create();
}

// ------------------------------------------------------------------------------------------------
//FrameBuffer::FrameBuffer( FrameBuffer &&other )
//: FrameBuffer()
//{
//	std::swap( _colorAttachmentCount,   other._colorAttachmentCount );
//	std::swap( _colorAttachments,       other._colorAttachments );
//	std::swap( _depthAttachment,        other._depthAttachment );
//	std::swap( _depthStencilAttachment, other._depthStencilAttachment);
//
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//}

// ------------------------------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
	destroy();
}

// ------------------------------------------------------------------------------------------------
//FrameBuffer& FrameBuffer::operator=( FrameBuffer &&other )
//{
//	reset();
//
//	std::swap( _colorAttachmentCount,   other._colorAttachmentCount );
//	std::swap( _colorAttachments,       other._colorAttachments );
//	std::swap( _depthAttachment,        other._depthAttachment );
//	std::swap( _depthStencilAttachment, other._depthStencilAttachment);
//
//
//	std::swap( _created,   other._created);
//	std::swap( _objectID,  other._objectID);
//	return *this;
//}

// -------------------------------------------------------------------------------------------------
void FrameBuffer::reset()
{
	OpenGLObject::reset();

	_changes                = Changes::None;
	_colorAttachmentCount   = 0;
	_depthAttachment        = {};
	_depthStencilAttachment = {};
	_colorAttachments       = std::vector<ColorAttachment>(10);
}

// -------------------------------------------------------------------------------------------------
int FrameBuffer::objectLabelIdentifier() const 
{
	return GL_FRAMEBUFFER;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::create()
{
	destroy();
	OpenGLObject::create();
	
	glGenFramebuffers( 1, &_objectID );
	glCheck;
	
	_created = _objectID != 0;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::destroy()
{
	if( !isCreated() )
		return;

	
	glDeleteFramebuffers( 1, &_objectID );
	glCheck;
	reset();
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::bind() const
{
	glBindFramebuffer( GL_FRAMEBUFFER, _objectID );
	glCheck;

	if( ( _changes & Changes::Color ) == Changes::Color )
	{
		std::vector<GLenum> drawBuffers;

		for( int i = 0; i < (int) _colorAttachments.size(); ++i )
		{
			if( _colorAttachments[i].changed )
			{
				makeGLAttachment( AttachmentPoint( ColorAttachment0 + i ), _colorAttachments[i].texture );
				_colorAttachments[i].changed = false;
			}

			if( _colorAttachments[i].texture )
				drawBuffers.push_back( GL_COLOR_ATTACHMENT0 + i );
		}
		glDrawBuffers( (int) drawBuffers.size(), &drawBuffers[0] );
		glCheck;
		_changes = Changes( _changes & ~Changes::Color );
	}

	if( ( _changes & Changes::Depth ) == Changes::Depth )
	{
		makeGLAttachment( DepthAttachment, _depthAttachment );
		_changes = Changes( _changes & ~Changes::Depth );
	}


	//
	// The depth-stencil attachment overrides the depth attachment:
	//
	//    "Attaching a level of a texture to GL_DEPTH_STENCIL_ATTACHMENT 
	//     is equivalent to attaching that level to both the 
	//     GL_DEPTH_ATTACHMENT and the GL_STENCIL_ATTACHMENT attachment 
	//     points simultaneously."
	//
	// We do not expose just a stencil attachment because TextureFormat
	// does not contain a stencil only format.
	if( ( _changes & Changes::DepthStencil ) == Changes::DepthStencil )
	{
		makeGLAttachment( DepthStencilAttachment, _depthStencilAttachment );
		_changes = Changes( _changes & ~Changes::DepthStencil );
	}
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::unbind() const
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glCheck;
}


// ------------------------------------------------------------------------------------------------
int  FrameBuffer::colorAttachmentCount()      const { return _colorAttachmentCount; }
bool FrameBuffer::hasDepthAttachment()        const { return _depthAttachment->isCreated(); }
bool FrameBuffer::hasDepthStencilAttachment() const { return _depthStencilAttachment->isCreated(); }
Texture2DPtr FrameBuffer::attachment( const AttachmentPoint &a ) const
{
	switch( a )
	{
	case ColorAttachment0:
	case ColorAttachment1:
	case ColorAttachment2:
	case ColorAttachment3:
	case ColorAttachment4:
	case ColorAttachment5:
	case ColorAttachment6:
	case ColorAttachment7:
	case ColorAttachment8:
	case ColorAttachment9:
		return _colorAttachments.at( a ).texture;
	}

	if( a == AttachmentPoint::DepthAttachment )        return _depthAttachment;
	if( a == AttachmentPoint::DepthStencilAttachment ) return _depthStencilAttachment;

	return nullptr;
}

// ------------------------------------------------------------------------------------------------
int  FrameBuffer::colorAttachmentDrawBufferIndex( const AttachmentPoint a ) const 
{ 
	assert( a < kMaxColorAttachment );

	if( !_colorAttachments[a].texture )
		return -1;

	int retval = 0;
	std::for_each( _colorAttachments.begin(), _colorAttachments.begin()+a, [&retval]( const auto &item ) { 
		if( item.texture ) ++retval;
	} );

	return retval; 
}

// ------------------------------------------------------------------------------------------------
/**
	the frame buffer must be bound before calling readPixels
*/
void FrameBuffer::readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat,
							  const ImageDataType pixelType, const Math::irect &roi, void *pixels ) const
{
	//bind();
	//glBindFramebuffer( GL_READ_FRAMEBUFFER, _objectID );
 
	glReadBuffer( glWrap(attachPoint) );
	glReadPixels( roi.left(), roi.bottom(), roi.width(), roi.height(), glWrap(pixelFormat), glWrap(pixelType), pixels );
	glCheck;

	//if( !_bound )
	//	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::attach( const AttachmentPoint &attachPoint, const Texture2DPtr &texture )
{
	switch( attachPoint )
	{
	case ColorAttachment0:
	case ColorAttachment1:
	case ColorAttachment2:
	case ColorAttachment3:
	case ColorAttachment4:
	case ColorAttachment5:
	case ColorAttachment6:
	case ColorAttachment7:
	case ColorAttachment8:
	case ColorAttachment9:
		if( texture && !texture->description().isColorRenderable() )
			assert( ("Texture must be color renderable but the Description.ColorRenderable property is false",0) );

		if( _colorAttachments[attachPoint].texture != texture )
		{
			if( _colorAttachments[attachPoint].texture && !texture )
			{
				--_colorAttachmentCount; //detach
			}
			else if( !_colorAttachments[attachPoint].texture && texture )
			{
				++_colorAttachmentCount;
			}

			_colorAttachments[attachPoint].texture = texture;
			_colorAttachments[attachPoint].changed = true;
			_changes                               = Changes( _changes | Changes::Color );
		}
		break;

	case DepthAttachment:
		if( _depthAttachment != texture )
		{
			if( texture && !texture->description().isDepthRenderable() )
				assert(("Texture must be depth renderable but the Description.DepthRenderable property is false",0) );

			_depthAttachment = texture;
			_changes         = Changes( _changes | Changes::Depth );
		}
		break;

	case DepthStencilAttachment:
		if( _depthStencilAttachment != texture )
		{
			if( texture && !texture->description().isDepthStencilRenderable() )
			{
				assert( ("Texture must be depth renderable but the Description.DepthRenderable property is false",0) );
				//throw new ArgumentException("Texture must be depth renderable but the Description.DepthRenderable property is false.");
			}

			_depthStencilAttachment = texture;
			_changes                = Changes( _changes | Changes::DepthStencil );
		}
		break;
	}
}

// ------------------------------------------------------------------------------------------------
std::string FrameBuffer::info()        const
{
	std::string ret( "Unknown error" );

	const GLenum r = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	glCheck;

	switch( r )
	{
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:         ret = "Incomplete attachment";  break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: ret = "Missing attachment"; break;
		//case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:        ret = "FBO Failed: Dimensions Error";  break;
		//case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:          ret = "FBO Failed: Formats Error";     break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:         ret = "Incomplete draw buffer"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:         ret = "Incomplete read buffer"; break;
	case GL_FRAMEBUFFER_UNSUPPORTED:                    ret = "Not supported";     break;
	case GL_FRAMEBUFFER_UNDEFINED:                      ret = "Undefined";         break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:         ret = "Incomplete multisample";     break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:       ret = "Incomplete layer targets";   break;
	case GL_FRAMEBUFFER_COMPLETE:                       ret = "No errors";                 break;
	}

	return "Framebuffer " + std::to_string( _objectID ) + " Status: " + ret;
}