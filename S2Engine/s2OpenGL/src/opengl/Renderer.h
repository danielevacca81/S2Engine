// Renderer.h
//
#ifndef RENDERER_ONCE
#define RENDERER_ONCE

#include "Primitive.h"
#include "VertexArray.h"
#include "Mesh.h"


namespace s2 {
namespace OpenGL {

// stateless renderer class. issues draw calls. internal use
class Renderer
{
public:
	Renderer() = delete;
	~Renderer() = delete;

	// @todo: set conditional rendering and queries

	static void draw( const Primitive &primitive, const VertexArray &va );
	static void draw( const Primitive &primitive, const Mesh &m );
};


}
}

#endif