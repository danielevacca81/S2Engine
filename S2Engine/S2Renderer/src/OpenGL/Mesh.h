// Mesh.h
//
#ifndef MESH_ONCE
#define MESH_ONCE

#include "S2RendererAPI.h"

#include "OpenGL/BufferObject.h"
#include "OpenGL/VertexArray.h"
#include "Common/Color.h"

#include "Math/Math.h"

#include <vector>

namespace OpenGL {

class S2RENDERER_API Mesh
{
public:
	enum PrimitiveType
	{
		Points,
		Lines,
		LineLoop,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan,
		LinesAdjacency,
		LineStripAdjacency,
		TrianglesAdjacency,
		TriangleStripAdjacency	
	};

	enum WindingOrder
	{
		Clockwise,
		Counterclockwise	
	};

	BufferObject::BufferUsageHint _usageHint;

private:
	VertexArray _va;

	friend class Renderer;
public:
	Mesh();
	~Mesh();

	void setVertices     ( const std::vector<Math::vec3>  &points   );
	void setColors       ( const std::vector<s2::Color>   &colors   );
	void setNormals      ( const std::vector<Math::vec3>  &normals  );
	void setTextureCoords( const std::vector<Math::vec2>  &texCoords);

	void setIndices( const std::vector<int> &indices );

	PrimitiveType primitiveType;
	WindingOrder  frontFaceWindingOrder;
};

}
#endif