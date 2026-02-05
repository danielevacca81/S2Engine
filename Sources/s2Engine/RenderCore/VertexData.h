// VertexData.h
//
#ifndef S2_RENDERCORE_VERTEXDATA_H
#define S2_RENDERCORE_VERTEXDATA_H

#include "s2Engine_API.h"

#include "BufferObject.h"
#include "VertexArray.h"
#include "VertexAttributeLocation.h"

#include "Graphics/Color.h"
#include "Geometry/MeshData.h"
#include "Math/Math.h"

#include <vector>
#include <memory>


namespace s2 {
namespace RenderCore {

class VertexData;
typedef std::shared_ptr<VertexData>   VertexDataPtr;

class S2ENGINE_API VertexData
{
public:
	// CANNOT BE SHARED BETWEEN CONTEXTS
	static VertexDataPtr New( const BufferObject::UsageHint& hint = BufferObject::UsageHint::StaticDraw );
	static VertexDataPtr New( const std::vector<Math::vec3>& points, const BufferObject::UsageHint& hint = BufferObject::UsageHint::StaticDraw );
	static VertexDataPtr New( const s2::MeshData3D& meshData, const BufferObject::UsageHint& hint = BufferObject::UsageHint::StaticDraw );

public:
	VertexData( const BufferObject::UsageHint& hint = BufferObject::UsageHint::StaticDraw );
	VertexData( const std::vector<Math::vec3>& points, const BufferObject::UsageHint& hint = BufferObject::UsageHint::StaticDraw );
	VertexData( const s2::MeshData3D& meshData, const BufferObject::UsageHint& hint = BufferObject::UsageHint::StaticDraw );

	void setVertices( const std::vector<Math::fvec3>& points );
	void setVertices( const std::vector<Math::fvec2>& points2D );
	void setColors  ( const std::vector<Color>& colors );
	void setNormals ( const std::vector<Math::fvec3>& normals );
	void setTextureCoords( const std::vector<Math::fvec2>& texCoords );
	void setIndices( const std::vector<uint32_t>& indices );

	void setAttribute( VertexAttributeLocation loc, const std::vector<uint32_t>& attrib );
	void setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec2>& attrib );
	void setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec3>& attrib );
	void setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec4>& attrib );

	void setAttribute( VertexAttributeLocation loc, const std::vector<float>& attrib );
	void setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec2>& attrib );
	void setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec3>& attrib );
	void setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec4>& attrib );


private:
	VertexArrayPtr _vao;

	friend class Renderer;
	//friend class Context;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_VERTEXDATA_H