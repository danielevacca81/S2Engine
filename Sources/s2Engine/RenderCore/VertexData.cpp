// VertexData.cpp
//
#include "VertexData.h"

#include "Core/VectorCast.h"


using namespace s2;
using namespace RenderCore;


// ------------------------------------------------------------------------------------------------
// Type traits for attribute configuration
// ------------------------------------------------------------------------------------------------
namespace detail {

// Component count traits
template<typename T> struct ComponentCount;
template<> struct ComponentCount<uint32_t>    { static constexpr int value = 1; };
template<> struct ComponentCount<Math::ivec2> { static constexpr int value = 2; };
template<> struct ComponentCount<Math::ivec3> { static constexpr int value = 3; };
template<> struct ComponentCount<Math::ivec4> { static constexpr int value = 4; };
template<> struct ComponentCount<float>       { static constexpr int value = 1; };
template<> struct ComponentCount<Math::fvec2> { static constexpr int value = 2; };
template<> struct ComponentCount<Math::fvec3> { static constexpr int value = 3; };
template<> struct ComponentCount<Math::fvec4> { static constexpr int value = 4; };
template<> struct ComponentCount<Color>       { static constexpr int value = 4; };

// Component data type traits
template<typename T> struct ComponentDataType;
template<> struct ComponentDataType<uint32_t>    { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::UnsignedInt; };
template<> struct ComponentDataType<Math::ivec2> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::UnsignedInt; };
template<> struct ComponentDataType<Math::ivec3> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::UnsignedInt; };
template<> struct ComponentDataType<Math::ivec4> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::UnsignedInt; };
template<> struct ComponentDataType<float>       { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::Float; };
template<> struct ComponentDataType<Math::fvec2> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::Float; };
template<> struct ComponentDataType<Math::fvec3> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::Float; };
template<> struct ComponentDataType<Math::fvec4> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::Float; };
template<> struct ComponentDataType<Color>       { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::Float; };

// Generic template method
template<typename T>
static inline 
void setAttribute( const VertexArrayPtr& vao, VertexAttributeLocation location, const std::vector<T>& data )
{
	// Compile-time check per tipi supportati
	static_assert( 
		std::is_same_v<T, uint32_t> ||
		std::is_same_v<T, float> ||
		std::is_same_v<T, Math::ivec2> ||
		std::is_same_v<T, Math::ivec3> ||
		std::is_same_v<T, Math::ivec4> ||
		std::is_same_v<T, Math::fvec2> ||
		std::is_same_v<T, Math::fvec3> ||
		std::is_same_v<T, Math::fvec4> ||
		std::is_same_v<T, Color>,
		"Unsupported attribute type"
	);

	const uint32_t numElements = static_cast<uint32_t>( data.size() );

	if( numElements == 0 )
		return;

	constexpr int componentCount = ComponentCount<T>::value;
	constexpr auto componentType = ComponentDataType<T>::value;

	VertexBuffer buf( numElements * sizeof( T ), vao->usageHint() );
	buf.sendData( const_cast<void*>( static_cast<const void*>( data.data() ) ), numElements * sizeof( T ), 0 );

	vao->attribute( location ).set( std::move( buf ), componentType, componentCount, false, 0, 0 );
}

} // namespace detail

// ------------------------------------------------------------------------------------------------
VertexDataPtr VertexData::New( const BufferObject::UsageHint &hint )                                        { return std::make_shared<VertexData>( hint ); }
VertexDataPtr VertexData::New( const std::vector<Math::vec3>& points, const BufferObject::UsageHint &hint ) { return std::make_shared<VertexData>( points,hint ); }
VertexDataPtr VertexData::New( const s2::MeshData3D &mesh, const BufferObject::UsageHint& hint )            { return std::make_shared<VertexData>( mesh, hint ); }

// ------------------------------------------------------------------------------------------------
VertexData::VertexData( const BufferObject::UsageHint &hint )
{
	_vao = VertexArray::New( hint );
}

// ------------------------------------------------------------------------------------------------
VertexData::VertexData( const std::vector<Math::vec3>& points, const BufferObject::UsageHint& hint )
{
	_vao = VertexArray::New( hint );
	setVertices( points );
}

// ------------------------------------------------------------------------------------------------
VertexData::VertexData( const s2::MeshData3D& meshData, const BufferObject::UsageHint& hint )
{
	_vao = VertexArray::New( hint );

	setVertices( s2::vector_cast<Math::dvec3, Math::vec3>( meshData.vertices ) );
	setNormals( s2::vector_cast<Math::dvec3,Math::vec3>( meshData.normals ) );
	setTextureCoords( s2::vector_cast<Math::dvec2,Math::vec2>( meshData.uvCoords ) );
	setIndices( meshData.indices );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setVertices(      const std::vector<Math::fvec3> &points )    { detail::setAttribute( _vao, VertexAttributeLocation::VA_Position, points ); }
void VertexData::setVertices(      const std::vector<Math::fvec2> &points2D )  { detail::setAttribute( _vao, VertexAttributeLocation::VA_Position, points2D ); }
void VertexData::setColors(        const std::vector<Color>       &colors )    { detail::setAttribute( _vao, VertexAttributeLocation::VA_Color,    colors ); }
void VertexData::setNormals(       const std::vector<Math::fvec3> &normals )   { detail::setAttribute( _vao, VertexAttributeLocation::VA_Normal,   normals ); }
void VertexData::setTextureCoords( const std::vector<Math::fvec2> &texCoords ) { detail::setAttribute( _vao, VertexAttributeLocation::VA_UVCoords, texCoords ); }

// ------------------------------------------------------------------------------------------------
void VertexData::setIndices( const std::vector<uint32_t>& indices )
{
	const uint32_t numIndices = static_cast<uint32_t>( indices.size() );

	if( numIndices == 0 )
		return;

	_vao->indexBuffer().set( numIndices * sizeof( unsigned int ), IndexBuffer::UnsignedInt, _vao->usageHint() );
	_vao->indexBuffer().sendData( const_cast<void*>( static_cast<const void*>( indices.data() ) ), numIndices * sizeof( unsigned int ), 0 );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<uint32_t>& attrib )    { detail::setAttribute( _vao, loc, attrib);}
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec2>& attrib ) { detail::setAttribute( _vao, loc, attrib);}
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec3>& attrib ) { detail::setAttribute( _vao, loc, attrib);}
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec4>& attrib ) { detail::setAttribute( _vao, loc, attrib);}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<float>& attrib )       { detail::setAttribute( _vao, loc, attrib);}
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec2>& attrib ) { detail::setAttribute( _vao, loc, attrib);}
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec3>& attrib ) { detail::setAttribute( _vao, loc, attrib);}
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec4>& attrib ) { detail::setAttribute( _vao, loc, attrib);}