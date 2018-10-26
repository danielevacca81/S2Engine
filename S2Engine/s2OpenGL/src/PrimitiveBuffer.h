// PrimitiveBuffer.h
//
#ifndef PRIMITIVEBUFFER_ONCE
#define PRIMITIVEBUFFER_ONCE

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

class PrimitiveBuffer;
typedef std::shared_ptr<PrimitiveBuffer>   PrimitiveBufferPtr;

// ------------------------------------------------------------------------------------------------
// PrimitiveBuffer Attribute location:
//	 (0) vertices
//   (1) colors
//   (2) normals
//   (3) texture coords
class S2OPENGL_API PrimitiveBuffer
{
public:
	enum class WindingOrder
	{
		Clockwise,
		Counterclockwise	
	};

public:
	// CANNOT BE SHARED BETWEEN CONTEXTS
	static PrimitiveBufferPtr makeNew();

public:
	PrimitiveBuffer();
	~PrimitiveBuffer();

	void setVertices     ( const std::vector<Math::vec3>  &points   );
	void setColors       ( const std::vector<Color>       &colors   );
	void setNormals      ( const std::vector<Math::vec3>  &normals  );
	void setTextureCoords( const std::vector<Math::vec2>  &texCoords);
	void setIndices( const std::vector<unsigned int> &indices );

	BufferObject::UsageHint usageHint;
	Primitive               primitiveType;
	WindingOrder            frontFaceWindingOrder;

private:
	VertexArrayPtr _va;

	friend class Renderer;
};

}
}
#endif