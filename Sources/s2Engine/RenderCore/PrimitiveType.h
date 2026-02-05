// PrimitiveType.h
//
#ifndef S2_RENDERCORE_PRIMITIVETYPE_H
#define S2_RENDERCORE_PRIMITIVETYPE_H

namespace s2 {
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


} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_PRIMITIVETYPE_H
