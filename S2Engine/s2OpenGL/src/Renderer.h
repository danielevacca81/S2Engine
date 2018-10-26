// Renderer.h
//
#ifndef RENDERER_ONCE
#define RENDERER_ONCE

#include "Primitive.h"
#include "VertexArray.h"
#include "Mesh.h"


namespace s2 {
namespace OpenGL {

// stateless rendering helper class. issues draw calls. for internal use
class Renderer
{
public:
	Renderer() = delete;
	~Renderer() = delete;

	// @todo: conditional rendering and queries

	static void draw( const Primitive &primitive, const VertexArrayPtr &va );
	static void draw( const Primitive &primitive, const MeshPtr &m );
};


}
}

#endif