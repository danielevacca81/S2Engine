// FrameBuffer.cpp
//
#include "FrameBuffer.h"

#include "ImageFormat.h"


#include "OpenGL.h" 
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"

#include <algorithm>

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
// Helper: Convert BufferBit to GLbitfield
static inline GLbitfield toGLBitfield( uint32_t bufferBits )
{
    return static_cast<GLbitfield>( bufferBits );
}

// ------------------------------------------------------------------------------------------------
// Helper: Convert BlitFilter to GLenum
static inline GLenum toGLFilter( FrameBuffer::BlitFilter filter )
{
    switch( filter )
    {
    case FrameBuffer::BlitFilter::Nearest: return GL_NEAREST;
    case FrameBuffer::BlitFilter::Linear:  return GL_LINEAR;
    default:                               return GL_NEAREST;
    }
}

// ------------------------------------------------------------------------------------------------
// DSA helper: attach texture to framebuffer without binding
static inline void makeGLAttachment( 
    GLuint fboID,
    const FrameBuffer::AttachmentPoint& attachPoint, 
    const Texture2DPtr& texture )
{
    const GLenum attachment = glWrap( attachPoint );
    
    if( texture && texture->isValid() )
    {
        // DSA: glNamedFramebufferTexture (OpenGL 4.5+)
        glNamedFramebufferTexture( fboID, attachment, texture->id(), 0 );
    }
    else
    {
        // Detach texture
        glNamedFramebufferTexture( fboID, attachment, 0, 0 );
    }
    
    glCheck;
}

// ------------------------------------------------------------------------------------------------
FrameBufferPtr FrameBuffer::New()
{
    return std::make_shared<FrameBuffer>();
}

// ------------------------------------------------------------------------------------------------
FrameBuffer::FrameBuffer()
    : _changes( Changes::None )
    , _colorAttachments( kMaxColorAttachment )
    , _colorAttachmentCount( 0 )
{
    create();
}

// ------------------------------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
    destroy();
}

// -------------------------------------------------------------------------------------------------
void FrameBuffer::reset()
{
    OpenGLObject::reset();

    _changes                = Changes::None;
    _colorAttachmentCount   = 0;
    _depthAttachment        = {};
    _depthStencilAttachment = {};
    _colorAttachments       = std::vector<ColorAttachment>( kMaxColorAttachment );
}

// -------------------------------------------------------------------------------------------------
int FrameBuffer::objectLabelIdentifier() const 
{
    return GL_FRAMEBUFFER;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::create()
{
    if( isValid() )
        return;

    OpenGLObject::create();
    
    // DSA: glCreateFramebuffers (OpenGL 4.5+)
    glCreateFramebuffers( 1, &_objectID );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::destroy()
{
    if( !isValid() )
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

    // Apply pending changes using DSA
    applyPendingChanges();
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::unbind() const
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::resizeAllAttachments( int width, int height )
{
    // Resize color attachments
    for( int i = 0; i < kMaxColorAttachment; ++i )
    {
        auto& colorAttachment = _colorAttachments[i];
        if( colorAttachment.texture )
        {
            // Resize texture (DSA - recreates storage)
            // Note: This will reset the texture content, so caller should re-upload if needed
            colorAttachment.texture->resize( width, height );
            glNamedFramebufferTexture( _objectID, glWrap( AttachmentPoint( ColorAttachment0 + i ) ), colorAttachment.texture->id(), 0 );
            glCheck;
            _changes = Changes( _changes | Changes::Color );
        }
    }

    // Resize depth attachment
    if( _depthAttachment )
    {
        _depthAttachment->resize( width, height );
        glNamedFramebufferTexture( _objectID, glWrap (DepthAttachment), _depthAttachment->id(), 0 );
        glCheck;
        _changes = Changes( _changes | Changes::Depth );
    }

    // Resize depth-stencil attachment
    if( _depthStencilAttachment )
    {
        _depthStencilAttachment->resize( width, height );
        glNamedFramebufferTexture( _objectID, glWrap( DepthStencilAttachment ), _depthStencilAttachment->id(), 0 );
        glCheck;
        _changes = Changes( _changes | Changes::DepthStencil );
    }
}


// ------------------------------------------------------------------------------------------------
void FrameBuffer::applyPendingChanges() const
{
    // Process color attachments
    if( (_changes & Changes::Color) == Changes::Color )
    {
        std::vector<GLenum> drawBuffers;
        drawBuffers.reserve( kMaxColorAttachment );

        for( int i = 0; i < kMaxColorAttachment; ++i )
        {
            if( _colorAttachments[i].changed )
            {
                const AttachmentPoint ap = static_cast<AttachmentPoint>( ColorAttachment0 + i );
                makeGLAttachment( _objectID, ap, _colorAttachments[i].texture );
                _colorAttachments[i].changed = false;
            }

            if( _colorAttachments[i].texture && _colorAttachments[i].texture->isValid() )
            {
                drawBuffers.push_back( GL_COLOR_ATTACHMENT0 + i );
            }
        }

        // DSA: Set draw buffers
        if( !drawBuffers.empty() )
        {
            glNamedFramebufferDrawBuffers( 
                _objectID, 
                static_cast<GLsizei>( drawBuffers.size() ), 
                drawBuffers.data() 
            );
            glCheck;
        }
        else
        {
            // No color attachments
            glNamedFramebufferDrawBuffer( _objectID, GL_NONE );
            glCheck;
        }

        _changes = Changes( _changes & ~Changes::Color );
    }

    // Process depth attachment
    if( (_changes & Changes::Depth) == Changes::Depth )
    {
        makeGLAttachment( _objectID, DepthAttachment, _depthAttachment );
        _changes = Changes( _changes & ~Changes::Depth );
    }

    // Process depth-stencil attachment
    if( (_changes & Changes::DepthStencil) == Changes::DepthStencil )
    {
        makeGLAttachment( _objectID, DepthStencilAttachment, _depthStencilAttachment );
        _changes = Changes( _changes & ~Changes::DepthStencil );
    }
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::attach( const AttachmentPoint& attachPoint, const Texture2DPtr& texture )
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
    {
        if( texture && !texture->description().isColorRenderable() )
        {
            assert( false && "Texture must be color renderable" );
            return;
        }

        const int index = attachPoint - ColorAttachment0;
        
        if( _colorAttachments[index].texture != texture )
        {
            // Update attachment count
            if( _colorAttachments[index].texture && !texture )
            {
                --_colorAttachmentCount; // Detach
            }
            else if( !_colorAttachments[index].texture && texture )
            {
                ++_colorAttachmentCount; // Attach
            }

            _colorAttachments[index].texture = texture;
            _colorAttachments[index].changed = true;
            _changes = Changes( _changes | Changes::Color );
        }
        break;
    }

    case DepthAttachment:
        if( _depthAttachment != texture )
        {
            if( texture && !texture->description().isDepthRenderable() )
            {
                assert( false && "Texture must be depth renderable" );
                return;
            }

            _depthAttachment = texture;
            _changes = Changes( _changes | Changes::Depth );
        }
        break;

    case DepthStencilAttachment:
        if( _depthStencilAttachment != texture )
        {
            if( texture && !texture->description().isDepthStencilRenderable() )
            {
                assert( false && "Texture must be depth-stencil renderable" );
                return;
            }

            _depthStencilAttachment = texture;
            _changes = Changes( _changes | Changes::DepthStencil );
        }
        break;
    }
}

// ------------------------------------------------------------------------------------------------
int FrameBuffer::colorAttachmentCount() const 
{ 
    return _colorAttachmentCount; 
}

// ------------------------------------------------------------------------------------------------
bool FrameBuffer::hasDepthAttachment() const 
{ 
    return _depthAttachment && _depthAttachment->isValid(); 
}

// ------------------------------------------------------------------------------------------------
bool FrameBuffer::hasDepthStencilAttachment() const 
{ 
    return _depthStencilAttachment && _depthStencilAttachment->isValid(); 
}

// ------------------------------------------------------------------------------------------------
Texture2DPtr FrameBuffer::attachment( const AttachmentPoint& a ) const
{
    if( a >= ColorAttachment0 && a <= ColorAttachment9 )
    {
        const int index = a - ColorAttachment0;
        return _colorAttachments[index].texture;
    }

    if( a == AttachmentPoint::DepthAttachment )
        return _depthAttachment;
        
    if( a == AttachmentPoint::DepthStencilAttachment )
        return _depthStencilAttachment;

    return nullptr;
}

// ------------------------------------------------------------------------------------------------
int FrameBuffer::colorAttachmentDrawBufferIndex( const AttachmentPoint a ) const 
{ 
    assert( a >= ColorAttachment0 && a <= ColorAttachment9 );

    const int index = a - ColorAttachment0;
    
    if( !_colorAttachments[index].texture )
        return -1;

    // Count active attachments before this one
    int drawBufferIndex = 0;
    for( int i = 0; i < index; ++i )
    {
        if( _colorAttachments[i].texture && _colorAttachments[i].texture->isValid() )
            ++drawBufferIndex;
    }

    return drawBufferIndex; 
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const Math::irect& roi, float* pixels ) const
{
    readPixels( attachPoint, pixelFormat, ImageDataType::Float, roi, pixels );
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const Math::irect& roi, uint8_t* pixels ) const
{
    readPixels( attachPoint, pixelFormat, ImageDataType::UnsignedByte, roi, pixels );
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const ImageDataType pixelType, const Math::irect& roi, void* pixels ) const
{
    assert( isValid() && "FrameBuffer must be created before reading pixels" );
    assert( pixels && "Pixel buffer cannot be null" );

    // Apply any pending changes first
    applyPendingChanges();

    // DSA: Set read buffer for this framebuffer
    glNamedFramebufferReadBuffer( _objectID, glWrap( attachPoint ) );
    glCheck;

    // Bind for reading (required for glReadPixels)
    glBindFramebuffer( GL_READ_FRAMEBUFFER, _objectID );
    glCheck;

    // Read pixels
    glReadPixels( 
        roi.left(), 
        roi.bottom(), 
        roi.width(), 
        roi.height(), 
        glWrap( pixelFormat ), 
        glWrap( pixelType ), 
        pixels 
    );
    glCheck;

    // Unbind
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::blitTo( 
    const FrameBufferPtr& destination,
    const Math::irect& srcRect,
    const Math::irect& dstRect,
    BufferBit bufferBit,
    BlitFilter filter ) const
{
    blitTo( destination, srcRect, dstRect, static_cast<uint32_t>(bufferBit), filter );
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::blitTo( 
    const FrameBufferPtr& destination,
    const Math::irect& srcRect,
    const Math::irect& dstRect,
    uint32_t bufferBits,
    BlitFilter filter ) const
{
    assert( isValid() && "Source framebuffer must be created" );
    assert( destination && destination->isValid() && "Destination framebuffer must be created" );

    // Apply pending changes
    applyPendingChanges();
    destination->applyPendingChanges();

    const GLbitfield mask = toGLBitfield( bufferBits );
    const GLenum glFilter = toGLFilter( filter );

    // DSA: Named framebuffer blit (OpenGL 4.5+)
    glBlitNamedFramebuffer(
        _objectID,
        destination->id(),
        srcRect.left(), srcRect.bottom(), srcRect.right(), srcRect.top(),
        dstRect.left(), dstRect.bottom(), dstRect.right(), dstRect.top(),
        mask,
        glFilter
    );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::clear( BufferBit bufferBit )
{
    clear( static_cast<uint32_t>(bufferBit) );
}

// ------------------------------------------------------------------------------------------------
void FrameBuffer::clear( uint32_t bufferBits )
{
    assert( isValid() && "FrameBuffer must be created before clearing" );

    // Apply pending changes
    applyPendingChanges();

    const GLbitfield mask = toGLBitfield( bufferBits );

    // Bind and clear (glClear requires binding)
    glBindFramebuffer( GL_FRAMEBUFFER, _objectID );
    glCheck;

    glClear( mask );
    glCheck;

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
bool FrameBuffer::checkStatus() const
{
    assert( isValid() && "FrameBuffer must be created before checking status" );

    // Apply pending changes before checking
    applyPendingChanges();

    // DSA: Check framebuffer status
    const GLenum status = glCheckNamedFramebufferStatus( _objectID, GL_FRAMEBUFFER );
    glCheck;

    return status == GL_FRAMEBUFFER_COMPLETE;
}

// ------------------------------------------------------------------------------------------------
std::string FrameBuffer::info() const
{
    if( !isValid() )
        return "Framebuffer not created";

    // Apply pending changes
    applyPendingChanges();

    // DSA: Check status
    const GLenum status = glCheckNamedFramebufferStatus( _objectID, GL_FRAMEBUFFER );
    glCheck;

    std::string statusStr;
    
    switch( status )
    {
    case GL_FRAMEBUFFER_COMPLETE:                      statusStr = "Complete"; break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:         statusStr = "Incomplete attachment";break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: statusStr = "Missing attachment";break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:        statusStr = "Incomplete draw buffer";break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:        statusStr = "Incomplete read buffer";break;
    case GL_FRAMEBUFFER_UNSUPPORTED:                   statusStr = "Unsupported";break;
    case GL_FRAMEBUFFER_UNDEFINED:                     statusStr = "Undefined";break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:        statusStr = "Incomplete multisample";break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:      statusStr = "Incomplete layer targets";break;
    
    default:
        statusStr = "Unknown error (" + std::to_string( status ) + ")";
        break;
    }

    return "Framebuffer " + std::to_string( _objectID ) + " Status: " + statusStr;
}