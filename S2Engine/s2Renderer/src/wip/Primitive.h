// Primitive.h
//
#ifndef PRIMITIVE_ONCE
#define PRIMITIVE_ONCE

#include "s2Renderer_API.h"

//#include "VertexAttributeCollection.h"
#include "VertexAttribute.h"
#include "IndexBuffer.h"
#include "PrimitiveType.h"


namespace Renderer {

class S2RENDERER_API Primitive
{
public:
	VertexAttributeCollection vertexAttributes;
	IndexBuffer               indices;

	WindingOrder              windingOrder  = WindingOrder::Counterclockwise; // dv: usato?
	BufferObject::UsageHint   usageHint     = BufferObject::UsageHint::StaticDraw;

private:
	friend class RenderingHelper;
};

}
#endif