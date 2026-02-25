// RenderTarget.h
//
#ifndef S2_RENDERCORE_RENDERTARGET_H
#define S2_RENDERCORE_RENDERTARGET_H

#include "s2Engine_API.h"

#include "FrameBuffer.h"
#include "Texture.h"

#include <initializer_list>
#include <string>
#include <cstdint>

namespace s2 {
namespace RenderCore {

// ------------------------------------------------------------------------------------------------
// RenderTarget: High-level wrapper for FrameBuffer with automatic texture management
// 100% DSA - No binding required for setup
// Does NOT perform rendering operations (delegated to RenderCommands)
// ------------------------------------------------------------------------------------------------
class S2ENGINE_API RenderTarget
{
public:
    struct AttachmentDescription
    {
        FrameBuffer::AttachmentPoint attachPoint;
        TextureFormat                textureFormat;

        AttachmentDescription( FrameBuffer::AttachmentPoint ap, TextureFormat tf )
            : attachPoint( ap ), textureFormat( tf ) {}
    };

public:
    // Default constructor (RGBA8 color + Depth24)
    RenderTarget();
    
    // Custom attachments
    explicit RenderTarget( const std::initializer_list<AttachmentDescription>& attachments );

    // Label for debugging (OpenGL 4.3+)
    void setObjectLabel( const std::string& label );

    // ===== Dimensions Management =====
    
    Math::irect size()   const { return Math::irect( 0, 0, _width, _height ); }
    uint32_t    width()  const { return _width; }
    uint32_t    height() const { return _height; }
    
    // Resize all attachments (DSA)
    void resize( int32_t width, int32_t height );

    // ===== Attachment Management (DSA) =====
    
    // Attach existing texture (DSA)
    void attach( FrameBuffer::AttachmentPoint attachPoint, const Texture2DPtr& texture );
    
    // Create and attach new texture (DSA)
    void createAttachment( FrameBuffer::AttachmentPoint attachPoint, TextureFormat textureFormat );
    
    // Remove attachment (DSA)
    void removeAttachment( FrameBuffer::AttachmentPoint attachPoint );

    // Get attachment texture
    Texture2DPtr attachment( FrameBuffer::AttachmentPoint attachPoint ) const;
    
    // Get color attachment draw buffer index
    uint32_t colorAttachmentDrawBufferIndex( FrameBuffer::AttachmentPoint attachPoint ) const;

    // ===== Validation =====
    
    // Check if framebuffer is complete (DSA)
    bool isComplete() const;
    
    // Get status info
    std::string statusInfo() const;

    // ===== Direct FBO Access =====
    
    // Get underlying framebuffer (use for advanced operations)
    const FrameBufferPtr& framebuffer() const { return _fbo; }

private:
    std::string genLabelAttachment( FrameBuffer::AttachmentPoint attachPoint ) const;

private:
    uint32_t       _width  { 64 };
    uint32_t       _height { 64 };
    FrameBufferPtr _fbo;
};

} // namespace RenderCore
} // namespace s2

#endif // !S2_RENDERCORE_RENDERTARGET_H
