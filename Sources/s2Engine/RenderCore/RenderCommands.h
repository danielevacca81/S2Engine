// RenderCommands.h
//
#ifndef S2_RENDERCORE_RENDERCOMMANDS_H
#define S2_RENDERCORE_RENDERCOMMANDS_H

#include "s2Engine_API.h"

#include "PrimitiveType.h"
#include "PrimitiveBatch.h"
#include "VertexData.h"
#include "ClearState.h"
#include "DrawState.h"
#include "FrameBuffer.h"
#include "RenderTarget.h"
#include "Texture.h"

#include "Graphics/Pixmap.h"
#include "Math/Rectangle.h"

namespace s2 {
namespace RenderCore {

class Context;

// ------------------------------------------------------------------------------------------------
// Encapsulates low-level OpenGL rendering commands.
// Each Context owns a RenderCommands instance for executing rendering operations.
// ------------------------------------------------------------------------------------------------
class S2ENGINE_API RenderCommands
{
public:
	explicit RenderCommands( Context& context );
	~RenderCommands() = default;

	// Prevent copying
	RenderCommands( const RenderCommands& ) = delete;
	RenderCommands& operator=( const RenderCommands& ) = delete;

	// ===== RENDERING OPERATIONS =====

	// Clear operations
	void clear( const RenderTarget& target, const ClearState& cs = {} );
	void clear( const FrameBufferPtr& fbo, const ClearState& cs );

	// Draw operations
	void draw( const RenderTarget& target, const PrimitiveType& primitiveType,
			   const VertexArrayPtr& va, const DrawState& ds = {} );
	void draw( const RenderTarget& target, const PrimitiveType& primitiveType,
			   const VertexDataPtr& primitive, const DrawState& ds = {} );
	void draw( const RenderTarget& target, const PrimitiveBatch& batch, const DrawState& ds = {} );

	// Low-level draw (for special cases)
	void draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType,
			   const VertexArrayPtr& va, const DrawState& ds );
	void draw( const FrameBufferPtr& fbo, const PrimitiveType& primitiveType,
			   const VertexDataPtr& primitive, const DrawState& ds );

	// ===== READ OPERATIONS =====

	Pixmap<uint8_t> readPixels( const RenderTarget& target );
	Pixmap<uint8_t> readPixels( const FrameBufferPtr& fbo, uint32_t width, uint32_t height );

	void readPixels( const RenderTarget& target,
					 const FrameBuffer::AttachmentPoint& attachPoint,
					 const ImageFormat& pixelFormat,
					 const Math::irect& roi,
					 void* pixels );

	// ===== BLIT OPERATIONS =====

	void blit( const RenderTarget& source, const RenderTarget& destination,
			   const Math::irect& srcRect = {}, const Math::irect& dstRect = {} );
	void blit( const FrameBufferPtr& srcFBO, const FrameBufferPtr& dstFBO,
			   const Math::irect& srcRect, const Math::irect& dstRect = {} );

	void blitToScreen( const RenderTarget& source, const Math::irect& srcRect = {} );

	// ===== UTILITY OPERATIONS =====

	void drawFullscreenQuad( const Texture2DPtr& srcTexture );

	// Access to owning context
	Context& context() { return _context; }
	const Context& context() const { return _context; }

private:
	// Low-level OpenGL draw call (internal)
	void executeDrawCall( const PrimitiveType& primitive, const VertexArrayPtr& va );

	// Sanitize draw state with render target dimensions
	DrawState sanitizeDrawState( const DrawState& ds, const RenderTarget& target ) const;

private:
	Context& _context; // Reference to owning context
};

} // namespace RenderCore
} // namespace s2

#endif // !S2_RENDERCORE_RENDERCOMMANDS_H