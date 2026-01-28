// VertexAttributeFloatVector4.h
//
#ifndef VERTEXATTRIBUTEFLOATVECTOR4_ONCE
#define VERTEXATTRIBUTEFLOATVECTOR4_ONCE
#if 0
#include "s2Renderer_API.h"

#include "VertexAttribute.h"
#include "VertexAttributeType.h"

#include "Core/Math.h"

namespace Renderer {

class S2RENDERER_API VertexAttributeFloatVector4 : public VertexAttributeBase< Math::fvec4 >
{
public:
	VertexAttributeFloatVector4( const std::string &name, const std::vector< Math::fvec4 > &values = {} )
	: VertexAttributeBase( name, VertexAttributeType::FloatVector4, values )
	{}

	VertexAttributeFloatVector4( const std::string &name, int capacity )
	: VertexAttributeBase( name, VertexAttributeType::FloatVector4, capacity )
	{}
};

}
#endif
#endif