// VertexAttributeFloat.h
//
#ifndef VERTEXATTRIBUTEFLOAT_ONCE
#define VERTEXATTRIBUTEFLOAT_ONCE
#if 0
#include "s2Renderer_API.h"

#include "VertexAttribute.h"
#include "VertexAttributeType.h"

namespace Renderer {

class S2RENDERER_API VertexAttributeFloat : public VertexAttributeBase<float>
{
public:
	VertexAttributeFloat( const std::string &name, const std::vector<float> &values = {} )
	: VertexAttributeBase( name, VertexAttributeType::Float, values )
	{}

	VertexAttributeFloat( const std::string &name, int capacity )
	: VertexAttributeBase( name, VertexAttributeType::Float, capacity )
	{}
};

}
#endif
#endif