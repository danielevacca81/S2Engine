// PrimitiveBuffer.h
//
#ifndef PRIMITIVEBUFFER_ONCE
#define PRIMITIVEBUFFER_ONCE

#include "RenderSystem_API.h"

#include "PrimitiveType.h"
#include "BufferObject.h"
#include "VertexArray.h"

#include "Core/Color.h"
#include "Math/Math.h"

#include <vector>
#include <memory>

namespace RenderSystem {

class PrimitiveBuffer;
class Context;
typedef std::shared_ptr<PrimitiveBuffer>   PrimitiveBufferPtr;

// ------------------------------------------------------------------------------------------------
// PrimitiveBuffer Attribute location:
//	 (0) vertices
//   (1) colors
//   (2) normals
//   (3) texture coords
class RENDERSYSTEM_API PrimitiveBuffer
{
public:
	// CANNOT BE SHARED BETWEEN CONTEXTS
	static PrimitiveBufferPtr New( const BufferObject::UsageHint &hint = BufferObject::UsageHint::StaticDraw );

public:
	PrimitiveBuffer( const BufferObject::UsageHint &hint = BufferObject::UsageHint::StaticDraw );

	void setVertices             ( const std::vector<Math::vec3>   &points   );
	void setColors               ( const std::vector<Color>        &colors   );
	void setNormals              ( const std::vector<Math::vec3>   &normals  );
	void setTextureCoords        ( const std::vector<Math::vec2>   &texCoords);
	void setIndices              ( const std::vector<unsigned int> &indices  );
	void setIntAttribute         ( const std::vector<uint32_t>     &attrib   );
	void setIntVector2Attribute  ( const std::vector<Math::ivec2>  &attrib   );
	void setIntVector3Attribute  ( const std::vector<Math::ivec3>  &attrib   );
	void setIntVector4Attribute  ( const std::vector<Math::ivec4>  &attrib   );
	void setFloatAttribute       ( const std::vector<float>        &attrib   );
	void setFloatVector2Attribute( const std::vector<Math::fvec2>  &attrib   );
	void setFloatVector3Attribute( const std::vector<Math::fvec3>  &attrib   );
	void setFloatVector4Attribute( const std::vector<Math::fvec4>  &attrib   );

	//PrimitiveType           primitiveType;
	//WindingOrder            frontFaceWindingOrder;

private:
	VertexArrayPtr _vao;

	friend class RenderEngine;
};

}
#endif