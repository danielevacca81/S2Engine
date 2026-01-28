// VertexAttributeFloat.h
//
#ifndef VERTEXATTRIBUTEFLOAT_ONCE
#define VERTEXATTRIBUTEFLOAT_ONCE

#include "s2Renderer_API.h"

#include "VertexAttribute.h"
#include "VertexAttributeType.h"

namespace Renderer {

class S2RENDERER_API VertexAttributeFloat : VertexAttributeBase<float>
{
public:
	VertexAttributeFloat( const std::string &name )
	: VertexAttributeBase( name, VertexAttributeType::Float)
	{}

	VertexAttributeFloat( const std::string &name, int capacity )
	: VertexAttributeBase( name, VertexAttributeType::Float, capacity )
	{}
};

}
#endif