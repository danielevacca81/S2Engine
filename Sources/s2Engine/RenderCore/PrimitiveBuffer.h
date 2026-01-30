// PrimitiveBuffer.h
//
#ifndef PRIMITIVEBUFFER_H
#define PRIMITIVEBUFFER_H

#include "s2Engine_API.h"

#include "PrimitiveType.h"
#include "BufferObject.h"
#include "VertexArray.h"

#include "Graphics/Color.h"
#include "Math/Math.h"

#include <vector>
#include <memory>

namespace RenderCore {

class PrimitiveBuffer;
typedef std::shared_ptr<PrimitiveBuffer>   PrimitiveBufferPtr;

// ------------------------------------------------------------------------------------------------
// PrimitiveBuffer Attribute location:
//	 (0) vertices
//   (1) colors
//   (2) normals
//   (3) texture coords
class S2ENGINE_API PrimitiveBuffer // rename: GeometryBuffer?
{
public:
	// CANNOT BE SHARED BETWEEN CONTEXTS
	static PrimitiveBufferPtr New( const BufferObject::UsageHint &hint = BufferObject::UsageHint::StaticDraw );
	static PrimitiveBufferPtr New( const std::vector<Math::vec3>& points, const BufferObject::UsageHint &hint = BufferObject::UsageHint::StaticDraw );

public:
	PrimitiveBuffer( const BufferObject::UsageHint &hint = BufferObject::UsageHint::StaticDraw );
	PrimitiveBuffer( const std::vector<Math::vec3> &points, const BufferObject::UsageHint &hint = BufferObject::UsageHint::StaticDraw );

	void setVertices             ( const std::vector<Math::vec3>   &points   );
	void setVertices             ( const std::vector<Math::vec2>   &points2D );
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

	//WindingOrder            frontFaceWindingOrder;

private:
	VertexArrayPtr _vao;

	friend class Renderer;
	//friend class Context;
};

}
#endif