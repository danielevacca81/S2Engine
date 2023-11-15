// PrimitiveType.h
//
#ifndef PRIMITIVETYPE_ONCE
#define PRIMITIVETYPE_ONCE

namespace RenderSystem {

enum class PrimitiveType
{
	Points,
	Lines,
	LineLoop,
	LineStrip,
	Triangles,
	TriangleStrip,
	TriangleFan,
	Quads,
	QuadStrip,
};

enum class WindingOrder
{
	Clockwise,
	Counterclockwise	
};


}
#endif // !PRIMITIVE_ONCE
