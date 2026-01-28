// VertexAttributeFloatVector2.h
//
#ifndef VERTEXATTRIBUTEFLOATVECTOR2_ONCE
#define VERTEXATTRIBUTEFLOATVECTOR2_ONCE

#if 0
#include "s2Renderer_API.h"

#include "VertexAttribute.h"
#include "VertexAttributeType.h"

#include "Core/Math.h"

namespace Renderer {

class S2RENDERER_API VertexAttributeFloatVector2 : public VertexAttributeBase< Math::fvec2 >
{
public:
	VertexAttributeFloatVector2( const std::string &name, const std::vector< Math::fvec2 > &values = {} )
	: VertexAttributeBase( name, VertexAttributeType::FloatVector2, values )
	{}

	VertexAttributeFloatVector2( const std::string &name, int capacity )
	: VertexAttributeBase( name, VertexAttributeType::FloatVector2, capacity )
	{}
};

}
#endif
#endif