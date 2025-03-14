// Renderer.h
//
#ifndef RENDERER_H
#define RENDERER_H

#include "RenderCore_API.h"

#include "PrimitiveType.h"
#include "PrimitiveBatch.h"
#include "ClearState.h"
#include "DrawState.h"
#include "FrameBuffer.h"

namespace RenderCore {

// ------------------------------------------------------------------------------------------------
class Renderer /// find better name?
{
public:
	static void            clear( const FrameBufferPtr &fbo, const ClearState& cs );
	static void            draw( const FrameBufferPtr& fbo, const PrimitiveType &primitiveType, const VertexArrayPtr &va, const DrawState &ds );
	static void            draw( const FrameBufferPtr& fbo, const PrimitiveType &primitiveType, const PrimitiveBufferPtr &p, const DrawState &ds );
	static void            draw( const FrameBufferPtr& fbo, const PrimitiveBatch& batch, const DrawState& ds );
	static void            draw( uint32_t targetFBO, const PrimitiveType& primitiveType, const PrimitiveBufferPtr &primitive, const DrawState& ds );
	static Pixmap<uint8_t> readPixels( const FrameBufferPtr &fbo, uint32_t width, uint32_t height );
	
	static uint32_t defaultFrameBufferObject();
};

}

#endif // !RENDERER_H
