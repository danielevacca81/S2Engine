// RenderingHelper.h
//
#ifndef RENDERINGHELPER_ONCE
#define RENDERINGHELPER_ONCE

#include "PrimitiveBuffer.h"
#include "PrimitiveType.h"
#include "VertexArray.h"


namespace Renderer {

// stateless rendering helper class. issues draw calls. for internal use
class RenderingHelper
{
public:
	RenderingHelper() = delete;
	~RenderingHelper() = delete;

	// @todo: conditional rendering and queries

	static void draw( const PrimitiveType &primitive, const VertexArrayPtr &va );
	static void draw( const PrimitiveType &primitive, const PrimitiveBufferPtr &mesh );
	//static void draw( const PrimitiveType &primitive, const Primitive &mesh ); //@todo_dv: draw batched primitives
};


}
#endif