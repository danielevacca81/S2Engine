// Vertex.h
// 
#ifndef VERTEX_ONCE
#define VERTEX_ONCE

#include "S2CoreAPI.h"

#include "Math/Math.h"

#include "Renderer/Color.h"

namespace OpenGL {

/************************************************************************************************/
/*                                             Vertex                                           */
/************************************************************************************************/
class S2CORE_API Vertex
{
protected:
	Math::dvec3 _position;
	Math::vec3  _normal;
	Math::vec2  _texCoord;
	Color       _color;

	// Attributes?

public:
	// -----------------------------------------------------------------------------------------------
	Vertex()
	{}

	// -----------------------------------------------------------------------------------------------
	Vertex( const Math::dvec3 &p, 
			const Math::vec3 &n  = Math::vec3(),
			const Math::vec2 &uv = Math::vec2(),
			const Color &c = Color() )
	: _position(p)
	, _normal(n)
	, _texCoord(uv)
	, _color(c)
	{}

	// -----------------------------------------------------------------------------------------------
	~Vertex()
	{}

	// -----------------------------------------------------------------------------------------------
	Math::vec3  normal()   const { return _normal;   }
	Math::vec2  texCoord() const { return _texCoord; }
	Color       color()    const { return _color;    }
	Math::dvec3 position() const { return _position; }
};

} // namespace OpenGL