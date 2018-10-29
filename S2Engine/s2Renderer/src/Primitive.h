// Primitive.h
//
#ifndef PRIMITIVE_ONCE
#define PRIMITIVE_ONCE

namespace s2 {
namespace Renderer {

enum class Primitive
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

}}
#endif // !PRIMITIVE_ONCE
