// PrimitiveType.h
//
#ifndef PRIMITIVETYPE_H
#define PRIMITIVETYPE_H

namespace RenderCore {

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
#endif // !PRIMITIVETYPE_H
