// RenderTarget.cpp
//
#include "RenderTarget.h"

#include "Context.h"

#include <cassert>
#include <algorithm>

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
RenderTarget::RenderTarget()
    : RenderTarget( {
        { FrameBuffer::ColorAttachment0, TextureFormat::RedGreenBlueAlpha8 },
        { FrameBuffer::DepthAttachment,  TextureFormat::Depth24 }
    } )
{
    assert( Context::current() && "OpenGL context required" );
}

// ------------------------------------------------------------------------------------------------
RenderTarget::RenderTarget( const std::initializer_list<AttachmentDescription>& attachments )
    : _width( 64 )
    , _height( 64 )
    , _fbo( FrameBuffer::New() )
{
    assert( Context::current() && "OpenGL context required" );
    
    // Create attachments (DSA - no binding)
    for( const auto& desc : attachments )
        createAttachment( desc.attachPoint, desc.textureFormat );
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::setObjectLabel( const std::string& label )
{
#ifdef _DEBUG
    // Set FBO label
    _fbo->setObjectLabel( label );

    // Set labels for all attachments
    for( int i = FrameBuffer::ColorAttachment0; i <= FrameBuffer::DepthStencilAttachment; ++i )
    {
        const auto attachPoint = static_cast<FrameBuffer::AttachmentPoint>( i );
        Texture2DPtr texture = _fbo->attachment( attachPoint );
        
        if( texture )
            texture->setObjectLabel( genLabelAttachment( attachPoint ) );
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::resize( int32_t width, int32_t height )
{
    constexpr int32_t kMinSize = 1;

    // Clamp to valid range
    width  = std::max( width,  kMinSize );
    height = std::max( height, kMinSize );

    // Early exit if dimensions unchanged
    if( _width == width && _height == height )
        return;

    _width  = width;
    _height = height;

    // Resize all attachments (DSA - textures resize themselves without binding)
    for( int i = FrameBuffer::ColorAttachment0; i <= FrameBuffer::DepthStencilAttachment; ++i )
    {
        const auto attachPoint = static_cast<FrameBuffer::AttachmentPoint>( i );
        Texture2DPtr texture = _fbo->attachment( attachPoint );
        
        if( texture )
            texture->resize( _width, _height );
    }

	_fbo->_changes = FrameBuffer::Changes( FrameBuffer::Changes::Color | FrameBuffer::Changes::Depth | FrameBuffer::Changes::DepthStencil );
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::attach( FrameBuffer::AttachmentPoint attachPoint, const Texture2DPtr& texture )
{
    // Validate and resize texture if needed
    if( texture )
    {
        const auto desc = texture->description();
        
        // Resize texture to match render target dimensions (DSA)
        if( desc.width() != static_cast<int>( _width ) || 
            desc.height() != static_cast<int>( _height ) )
        {
            texture->resize( _width, _height );
        }
    }

    // Attach to FBO (DSA - no binding)
    _fbo->attach( attachPoint, texture );
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::createAttachment( 
    FrameBuffer::AttachmentPoint attachPoint, 
    TextureFormat textureFormat )
{
    // Create texture with current dimensions (DSA)
    auto texture = Texture2D::New( 
        TextureDescription( 
            _width, 
            _height, 
            textureFormat, 
            false,  // no mipmaps for render targets
            genLabelAttachment( attachPoint )
        )
    );

    // Configure texture for render target usage (DSA)
    texture->setMinFilter( Texture2D::MinFilter::Linear );
    texture->setMagFilter( Texture2D::MagFilter::Linear );
    texture->setWrapS( Texture2D::WrapMode::ClampToEdge );
    texture->setWrapT( Texture2D::WrapMode::ClampToEdge );

    // Attach to FBO (DSA - no binding)
    _fbo->attach( attachPoint, texture );

#ifdef _DEBUG
    texture->setObjectLabel( genLabelAttachment( attachPoint ) );
#endif
}

// ------------------------------------------------------------------------------------------------
void RenderTarget::removeAttachment( FrameBuffer::AttachmentPoint attachPoint )
{
    // Remove attachment (DSA - no binding)
    _fbo->attach( attachPoint, nullptr );
}

// ------------------------------------------------------------------------------------------------
Texture2DPtr RenderTarget::attachment( FrameBuffer::AttachmentPoint attachPoint ) const
{
    return _fbo->attachment( attachPoint );
}

// ------------------------------------------------------------------------------------------------
uint32_t RenderTarget::colorAttachmentDrawBufferIndex( FrameBuffer::AttachmentPoint attachPoint ) const
{
    // No binding needed - FBO is already DSA-configured
    return _fbo->colorAttachmentDrawBufferIndex( attachPoint );
}

// ------------------------------------------------------------------------------------------------
bool RenderTarget::isComplete() const
{
    // Check framebuffer completeness (DSA)
    return _fbo->checkStatus();
}

// ------------------------------------------------------------------------------------------------
std::string RenderTarget::statusInfo() const
{
    // Get detailed status info (DSA)
    return _fbo->info();
}

// ------------------------------------------------------------------------------------------------
std::string RenderTarget::genLabelAttachment( FrameBuffer::AttachmentPoint attachPoint ) const
{
    std::string label;
    
#ifdef _DEBUG
    label = _fbo->objectLabel();
    
    switch( attachPoint )
    {
    case FrameBuffer::DepthAttachment:
        label += "_DepthAttachment";
        break;
    case FrameBuffer::DepthStencilAttachment:
        label += "_DepthStencilAttachment";
        break;
    case FrameBuffer::ColorAttachment0:
    case FrameBuffer::ColorAttachment1:
    case FrameBuffer::ColorAttachment2:
    case FrameBuffer::ColorAttachment3:
    case FrameBuffer::ColorAttachment4:
    case FrameBuffer::ColorAttachment5:
    case FrameBuffer::ColorAttachment6:
    case FrameBuffer::ColorAttachment7:
    case FrameBuffer::ColorAttachment8:
    case FrameBuffer::ColorAttachment9:
        label += "_ColorAttachment" + std::to_string( attachPoint - FrameBuffer::ColorAttachment0 );
        break;
    default:
        label += "_UnknownAttachment";
        break;
    }
#endif
    
    return label;
}