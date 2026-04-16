// RendererBackend.h
//
#ifndef S2_RENDERCORE_RENDERERBACKEND_H
#define S2_RENDERCORE_RENDERERBACKEND_H

#include "s2Engine_API.h"

#include "PrimitiveType.h"
#include "PrimitiveBatch.h"
#include "VertexData.h"
#include "ClearState.h"
#include "DrawState.h"
#include "FrameBuffer.h"
#include "RenderTarget.h"
#include "Texture.h"
#include "GPUBufferObject.h"

#include "Graphics/Pixmap.h"
#include "Math/Rectangle.h"

namespace s2 {
namespace RenderCore {

class Context;

// ------------------------------------------------------------------------------------------------
// Encapsulates low-level OpenGL rendering commands.
// Each Context owns a RendererBackend instance for executing rendering operations.
// ------------------------------------------------------------------------------------------------
class S2ENGINE_API RendererBackend
{
public:
	explicit RendererBackend( Context& context );
	~RendererBackend() = default;

	// Prevent copying
	RendererBackend( const RendererBackend& ) = delete;
	RendererBackend& operator=( const RendererBackend& ) = delete;

	// ===== RENDERING OPERATIONS =====

	// Clear operations
	void clear( const RenderTarget& target, const ClearState& cs = {} ) const;
	void clear( const FrameBufferPtr& fbo, const ClearState& cs ) const;

	// Draw operations
	void draw( const RenderTarget& target, const PrimitiveType& primitiveType,const VertexArrayPtr& va, const DrawState& ds = {} ) const;
	void draw( const RenderTarget& target, const PrimitiveType& primitiveType,const VertexDataPtr& primitive, const DrawState& ds = {} ) const;
	void draw( const RenderTarget& target, const PrimitiveBatch& batch, const DrawState& ds = {} ) const;		
	void drawRange( const RenderTarget& target, const PrimitiveType& primitiveType, const VertexArrayPtr& va, uint32_t elementCount, uint32_t elementOffset, uint32_t baseVertexOffset, const DrawState& ds = {} ) const;

	// Low-level draw (for special cases)
	void draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType, const VertexArrayPtr& va, const DrawState& ds ) const;
	void draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType,const VertexDataPtr& primitive, const DrawState& ds ) const;

	// ===== READ OPERATIONS =====
	Pixmap<uint8_t> readPixels( const RenderTarget& target ) const;
	Pixmap<uint8_t> readPixels( const FrameBufferPtr& fbo, uint32_t width, uint32_t height ) const;

	void readPixels( const RenderTarget& target,
					 const FrameBuffer::AttachmentPoint& attachPoint,
					 const ImageFormat& imageFormat,
					 const ImageDataType& pixelType,
					 const Math::irect& roi,
					 void* pixels ) const;

	// Async (non-blocking) readback of the entire render target into a PixelPackBuffer PBO.
	// The GPU writes into the PBO asynchronously; the caller must map the PBO on a later
	// frame to retrieve the data without stalling the pipeline.
	// Precondition: pbo must be of Type::PixelPackBuffer and sized >= width*height*pixelStride.
	void readPixelsAsync( const RenderTarget& target,
	                      const FrameBuffer::AttachmentPoint& attachPoint,
	                      const ImageFormat& imageFormat,
	                      const ImageDataType& pixelType,
	                      const GPUBufferObjectPtr& pbo ) const;

	
	// ===== BLIT OPERATIONS =====
	void blit( const RenderTarget& source, const RenderTarget& destination,
			   const Math::irect& srcRect = {}, const Math::irect& dstRect = {} ) const;
	void blit( const FrameBufferPtr& srcFBO, const FrameBufferPtr& dstFBO,
			   const Math::irect& srcRect, const Math::irect& dstRect = {} ) const;

	void blitToScreen( const RenderTarget& source, const Math::irect& srcRect = {} ) const;

	// ===== UTILITY OPERATIONS =====
	void flush() const;
	void drawFullscreenQuad( const Texture2DPtr& srcTexture ) const;

	// Access to owning context
	const Context& context() const { return _context; }


private:
	Context& _context; // Reference to owning context
};

} // namespace RenderCore
} // namespace s2

#endif // !S2_RENDERCORE_RENDERCOMMANDS_H