// VertexAttributeByte.h
//
#ifndef VERTEXATTRIBUTEBYTE_ONCE
#define VERTEXATTRIBUTEBYTE_ONCE
#if 0
#include "s2Renderer_API.h"

#include "VertexAttribute.h"
#include "VertexAttributeType.h"

namespace Renderer {

class S2RENDERER_API VertexAttibuteByte : public VertexAttributeBase<uint8_t>
{
public:
	VertexAttibuteByte( const std::string &name, const std::vector<uint8_t> &values = {} )
	: VertexAttributeBase( name, VertexAttributeType::UnsignedByte, values )
	{}

	VertexAttibuteByte( const std::string &name, int capacity )
	: VertexAttributeBase( name, VertexAttributeType::UnsignedByte, capacity )
	{}
};

}
#endif
#endif