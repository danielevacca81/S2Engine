// VertexAttributeFloatVector3.h
//
#ifndef VERTEXATTRIBUTEFLOATVECTOR3_ONCE
#define VERTEXATTRIBUTEFLOATVECTOR3_ONCE

#if 0
#include "s2Renderer_API.h"

#include "VertexAttribute.h"
#include "VertexAttributeType.h"

#include "Core/Math.h"

namespace Renderer {

class S2RENDERER_API VertexAttributeFloatVector3 : public VertexAttributeBase< Math::fvec3 >
{
public:
	VertexAttributeFloatVector3( const std::string &name, const std::vector< Math::fvec3 > &values = {} )
	: VertexAttributeBase( name, VertexAttributeType::FloatVector3, values )
	{}

	VertexAttributeFloatVector3( const std::string &name, int capacity )
	: VertexAttributeBase( name, VertexAttributeType::FloatVector3, capacity )
	{}
};

}
#endif
#endif