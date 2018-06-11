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
	Renderer() = default;
	~Renderer() = default;

	// @todo: set conditional rendering and queries

	void draw( const PrimitiveType &primitive, const VertexArray &va ) const;
	void draw( const PrimitiveType &primitive, const Mesh &m ) const;
};


}
}

#endif