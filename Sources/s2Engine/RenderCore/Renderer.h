// Renderer.h
//
#ifndef S2_RENDERCORE_RENDERER_H
#define S2_RENDERCORE_RENDERER_H

#include "s2Engine_API.h"

#include "Math/Rectangle.h"

#include "PrimitiveType.h"
#include "PrimitiveBatch.h"
#include "VertexData.h"

#include "ClearState.h"
#include "DrawState.h"
#include "FrameBuffer.h"
#include "RenderTarget.h"

namespace s2 {
namespace RenderCore {

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API Renderer /// find better name? RenderingEngine? DrawEngine? InternalRenderer?
{
public:
	static void            clear( const FrameBufferPtr &fbo, const ClearState& cs );
	static void            draw( const FrameBufferPtr& fbo, const PrimitiveType &primitiveType, const VertexArrayPtr &va, const DrawState &ds );
	static void            draw( const FrameBufferPtr& fbo, const PrimitiveType &primitiveType, const VertexDataPtr &p, const DrawState &ds );
	static void            draw( const FrameBufferPtr& fbo, const PrimitiveBatch& batch, const DrawState& ds );
	static void            draw( uint32_t targetFBO, const PrimitiveType& primitiveType, const VertexDataPtr &primitive, const DrawState& ds );
	static void            drawFullscreenQuad( const Texture2DPtr& srcTexture );
	static Pixmap<uint8_t> readPixels( const FrameBufferPtr &fbo, uint32_t width, uint32_t height );
	
	//static uint32_t defaultFrameBufferObject();

	static void blit( const FrameBufferPtr& srcFBO,
					  const FrameBufferPtr& dstFBO,
					  const Math::irect &srcRect,
					  const Math::irect& dstRect = {} );
};

} // namespace RenderCore
} // namespace s2

#endif // !S2_RENDERCORE_RENDERER_H
