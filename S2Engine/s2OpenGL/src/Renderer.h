// RenderingHelper.h
//
#ifndef RENDERINGHELPER_ONCE
#define RENDERINGHELPER_ONCE

#include "Primitive.h"
#include "VertexArray.h"
#include "PrimitiveBuffer.h"


namespace s2 {
namespace OpenGL {

// stateless rendering helper class. issues draw calls. for internal use
class RenderingHelper
{
public:
	RenderingHelper() = delete;
	~RenderingHelper() = delete;

	// @todo: conditional rendering and queries

	static void draw( const Primitive &primitive, const VertexArrayPtr &va );
	static void draw( const Primitive &primitive, const PrimitiveBufferPtr &m );
};


}
}

#endif