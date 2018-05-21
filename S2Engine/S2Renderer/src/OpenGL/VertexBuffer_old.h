// VertexBuffer.h
// 
#ifndef VERTEXBUFFER_ONCE
#define VERTEXBUFFER_ONCE

#include "S2RendererAPI.h"

#include "Math/Math.h"
#include "Graphics/Color.h"

#include <vector>

namespace OpenGL {

/************************************************************************************************/
/*                                             Vertex                                           */
/************************************************************************************************/
struct S2RENDERER_API Vertex
{
	Math::dvec3 position;
	Math::vec3  normal;
	Math::vec2  texCoord;
	s2::Color   color;

	// Attributes?

	// -----------------------------------------------------------------------------------------------
	Vertex()
	{}

	// -----------------------------------------------------------------------------------------------
	Vertex( const Math::dvec3 &p, 
		const Math::vec3 &n  = Math::vec3(),
		const Math::vec2 &uv = Math::vec2(),
		const s2::Color &c   = s2::Color() )
		: position(p)
		, normal(n)
		, texCoord(uv)
		, color(c)
	{}
};

/************************************************************************************************/
/*                                         VertexBuffer                                         */
/************************************************************************************************/
class S2RENDERER_API VertexBuffer
{
private:
	std::vector< Vertex > _vertices;

public:
	VertexBuffer();
	~VertexBuffer();

	const std::vector< Vertex > &vertices() const;
	Vertex                      &vertex( int i );
	int                          size() const;
	bool                         hasIndexBuffer() const;
	VertexBuffer                &operator << ( const Vertex &v );
	VertexBuffer                &operator << ( const VertexBuffer &vb );
};

} // namespace OpenGL

#endif