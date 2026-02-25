// FrameBuffer.h
//
#ifndef S2_RENDERCORE_FRAMEBUFFER_H
#define S2_RENDERCORE_FRAMEBUFFER_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"

#include "Texture.h"

#include "Math/Rectangle.h"

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace s2 {
namespace RenderCore {

class Context;

/************************************************************************************************/
/*                                       FrameBuffer                                            */
/************************************************************************************************/
class FrameBuffer;
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

class S2ENGINE_API FrameBuffer : public OpenGLObject
{
public:
    enum AttachmentPoint
    {
        ColorAttachment0 = 0,
        ColorAttachment1,
        ColorAttachment2,
        ColorAttachment3,
        ColorAttachment4,
        ColorAttachment5,
        ColorAttachment6,
        ColorAttachment7,
        ColorAttachment8,
        ColorAttachment9,
        DepthAttachment,
        DepthStencilAttachment
    };

    // Wrapper for GLbitfield
    enum class BufferBit : uint32_t
    {
        Color   = 0x00004000, // GL_COLOR_BUFFER_BIT
        Depth   = 0x00000100, // GL_DEPTH_BUFFER_BIT
        Stencil = 0x00000400  // GL_STENCIL_BUFFER_BIT
    };

    // Wrapper for filter
    enum class BlitFilter
    {
        Nearest,
        Linear
    };

public:
    // CANNOT BE SHARED BETWEEN CONTEXTS
    static FrameBufferPtr New();

public:
    FrameBuffer();
    ~FrameBuffer();

    void create()  override;
    void destroy() override;
    void bind()    const;
    void unbind()  const;

    // Attachment management
    void attach( const AttachmentPoint& attachPoint, const Texture2DPtr& texture );

    // Query attachments
    int  colorAttachmentCount() const;
    bool hasDepthAttachment() const;
    bool hasDepthStencilAttachment() const;

    Texture2DPtr attachment( const AttachmentPoint& a ) const;
    int          colorAttachmentDrawBufferIndex( const AttachmentPoint a ) const;	

    // Read pixels - convenience overloads
    void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const Math::irect& roi, float* pixels ) const;
    void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const Math::irect& roi, uint8_t* pixels ) const;
    void readPixels( const AttachmentPoint attachPoint, const ImageFormat pixelFormat, const ImageDataType pixelType, const Math::irect& roi, void* pixels ) const;

    // Check framebuffer completeness (DSA)
    bool checkStatus() const;

    // Blit between framebuffers (DSA)
    void blitTo( const FrameBufferPtr& destination,
                 const Math::irect& srcRect,
                 const Math::irect& dstRect,
                 BufferBit bufferBit,
                 BlitFilter filter = BlitFilter::Nearest ) const;

    // Multi-bit blit (for combining Color|Depth|Stencil)
    void blitTo( const FrameBufferPtr& destination,
                 const Math::irect& srcRect,
                 const Math::irect& dstRect,
                 uint32_t bufferBits, // Combination of BufferBit values
                 BlitFilter filter = BlitFilter::Nearest ) const;

    // Clear framebuffer - using wrapper types
    void clear( BufferBit bufferBit );
    void clear( uint32_t bufferBits ); // Combination of BufferBit values

    // Status info
    std::string info() const;

private:
    // DSA support: Apply pending attachment changes without explicit binding
    void applyPendingChanges() const;

    void reset() override;
    int  objectLabelIdentifier() const override;

private:
    enum Changes
    {
        None         = 0,
        Color        = 1,
        Depth        = 2,
        DepthStencil = 4
    };

    struct ColorAttachment
    {
        Texture2DPtr texture;
        bool         changed { false };

        ColorAttachment() = default;
    };

private:
    static constexpr int kMaxColorAttachment = ColorAttachment9 + 1;

    int          _colorAttachmentCount;
    Texture2DPtr _depthAttachment;
    Texture2DPtr _depthStencilAttachment;

    mutable std::vector<ColorAttachment> _colorAttachments;
    mutable Changes                      _changes;
};

// Bitwise operators for BufferBit
inline uint32_t operator|( FrameBuffer::BufferBit a, FrameBuffer::BufferBit b )
{
    return static_cast<uint32_t>(a) | static_cast<uint32_t>(b);
}

inline uint32_t operator|( uint32_t a, FrameBuffer::BufferBit b )
{
    return a | static_cast<uint32_t>(b);
}

} // namespace RenderCore
} // namespace s2

#endif //!S2_RENDERCORE_FRAMEBUFFER_H