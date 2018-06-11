// Mesh.h
//
#ifndef MESH_ONCE
#define MESH_ONCE

#include "s2OpenGL_API.h"

#include "Primitive.h"
#include "BufferObject.h"
#include "VertexArray.h"

#include "graphics/Color.h"
#include "math/Math.h"

#include <vector>

namespace s2 {
namespace OpenGL {

// ------------------------------------------------------------------------------------------------
// Mesh Attribute location:
//	 (0) vertices
//   (1) colors
//   (2) normals
//   (3) texture coords


class S2OPENGL_API Mesh
{
public:
	enum WindingOrder
	{
		Clockwise,
		Counterclockwise	
	};


public:
	Mesh();
	~Mesh();

	void setVertices     ( const std::vector<Math::vec3>  &points   );
	void setColors       ( const std::vector<Color>       &colors   );
	void setNormals      ( const std::vector<Math::vec3>  &normals  );
	void setTextureCoords( const std::vector<Math::vec2>  &texCoords);

	void setIndices( const std::vector<int> &indices );

	BufferObject::BufferUsageHint usageHint;
	PrimitiveType                 primitiveType;
	WindingOrder                  frontFaceWindingOrder;

private:
	VertexArray _va;

	friend class Renderer;
};

}
}
#endif