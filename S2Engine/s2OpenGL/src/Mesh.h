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
#include <memory>

namespace s2 {
namespace OpenGL {

class Mesh;
typedef std::shared_ptr<Mesh>   MeshPtr;

// ------------------------------------------------------------------------------------------------
// Mesh Attribute location:
//	 (0) vertices
//   (1) colors
//   (2) normals
//   (3) texture coords
class S2OPENGL_API Mesh
{
public:
	enum class WindingOrder
	{
		Clockwise,
		Counterclockwise	
	};

public:
	static MeshPtr New();
public:
	Mesh();
	~Mesh();

	VertexArray vao()const { return _va; }
	void setVertices     ( const std::vector<Math::vec3>  &points   );
	void setColors       ( const std::vector<Color>       &colors   );
	void setNormals      ( const std::vector<Math::vec3>  &normals  );
	void setTextureCoords( const std::vector<Math::vec2>  &texCoords);
	void setIndices( const std::vector<unsigned int> &indices );

	BufferObject::UsageHint usageHint;
	Primitive     primitiveType;
	WindingOrder  frontFaceWindingOrder;

private:
	VertexArray _va;

	friend class Renderer;
};

}
}
#endif