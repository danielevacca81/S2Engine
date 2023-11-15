// RenderEngine.h
//
#ifndef RENDERENGINE_ONCE
#define RENDERENGINE_ONCE

#include "RenderSystem_API.h"

#include "PrimitiveType.h"
#include "PrimitiveBatch.h"
#include "ClearState.h"
#include "DrawState.h"
#include "FrameBuffer.h"

namespace RenderSystem {

// ------------------------------------------------------------------------------------------------
class RENDERSYSTEM_API RenderEngine
{
public:
	static void clear( const FrameBufferPtr &fbo, const ClearState& cs );
	static void draw( const FrameBufferPtr& fbo, const PrimitiveType &primitiveType, const VertexArrayPtr &va, const DrawState &ds );
	static void draw( const FrameBufferPtr& fbo, const PrimitiveType &primitiveType, const PrimitiveBufferPtr &p, const DrawState &ds );
	static void draw( const FrameBufferPtr& fbo, const PrimitiveBatch& batch, const DrawState& ds );
	static void draw( uint32_t targetFBO, const PrimitiveType& primitiveType, const PrimitiveBufferPtr &primitive, const DrawState& ds );

	//void disableShadowingOneShot() { _stateManager.disableShadowingOneShot(); }

};

}

#endif // !RENDERENGINE_ONCE
