// Primitive.h
//
#ifndef PRIMITIVE_ONCE
#define PRIMITIVE_ONCE

namespace s2 {
namespace OpenGL {

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
	//LinesAdjacency,
	//LineStripAdjacency,
	//TrianglesAdjacency,
	//TriangleStripAdjacency
};

}}
#endif // !PRIMITIVE_ONCE
