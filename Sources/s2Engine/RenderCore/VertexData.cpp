// VertexData.cpp
//
#include "VertexData.h"

#include "Core/VectorCast.h"

#include <cassert>

namespace s2 {
namespace RenderCore {

// ------------------------------------------------------------------------------------------------
// Type traits for attribute configuration
// ------------------------------------------------------------------------------------------------
namespace detail {

// Component count traits
template<typename T> struct ComponentCount;
template<> struct ComponentCount<uint32_t> { static constexpr int value = 1; };
template<> struct ComponentCount<Math::ivec2> { static constexpr int value = 2; };
template<> struct ComponentCount<Math::ivec3> { static constexpr int value = 3; };
template<> struct ComponentCount<Math::ivec4> { static constexpr int value = 4; };
template<> struct ComponentCount<float> { static constexpr int value = 1; };
template<> struct ComponentCount<Math::fvec2> { static constexpr int value = 2; };
template<> struct ComponentCount<Math::fvec3> { static constexpr int value = 3; };
template<> struct ComponentCount<Math::fvec4> { static constexpr int value = 4; };
template<> struct ComponentCount<Color> { static constexpr int value = 4; };

// Component data type traits
template<typename T> struct ComponentDataType;
template<> struct ComponentDataType<uint32_t> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::UnsignedInt; };
template<> struct ComponentDataType<Math::ivec2> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::Int; };
template<> struct ComponentDataType<Math::ivec3> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::Int; };
template<> struct ComponentDataType<Math::ivec4> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::Int; };
template<> struct ComponentDataType<float> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::Float; };
template<> struct ComponentDataType<Math::fvec2> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::Float; };
template<> struct ComponentDataType<Math::fvec3> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::Float; };
template<> struct ComponentDataType<Math::fvec4> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::Float; };
template<> struct ComponentDataType<Color> { static constexpr AttributeBuffer::ComponentDatatype value = AttributeBuffer::ComponentDatatype::Float; };

// ------------------------------------------------------------------------------------------------
// Generic template method for DSA attribute setup
// ------------------------------------------------------------------------------------------------
template<typename T>
static inline void setAttribute(
    const VertexArrayPtr& vao,
    VertexAttributeLocation location,
    const std::vector<T>& data )
{
    // Compile-time check for supported types
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

    const int64_t bufferSize = numElements * sizeof( T );

    // Create GPU buffer (DSA)
    auto gpuBuffer = GPUBufferObject::New(
        bufferSize,
        GPUBufferObject::Type::ArrayBuffer,
        vao->usageHint()
    );

    // Upload data (DSA)
    gpuBuffer->setData( data.data(), bufferSize, 0 );

    // Create attribute buffer
    AttributeBuffer attr;
    attr.set(
        gpuBuffer,
        componentType,
        componentCount,
        false,  // normalize
        0,      // offset
        0       // stride (auto-calculated)
    );

    // Set attribute using DSA (no VAO binding)
    vao->setAttribute( location, attr );
}
} // namespace detail

// ------------------------------------------------------------------------------------------------
VertexDataPtr VertexData::New( const GPUBufferObject::UsageHint& hint )
{
    return std::make_shared<VertexData>( hint );
}

// ------------------------------------------------------------------------------------------------
VertexDataPtr VertexData::New( const std::vector<Math::vec3>& points, const GPUBufferObject::UsageHint& hint )
{
    return std::make_shared<VertexData>( points, hint );
}

// ------------------------------------------------------------------------------------------------
VertexDataPtr VertexData::New( const s2::MeshData3D& mesh, const GPUBufferObject::UsageHint& hint )
{
    return std::make_shared<VertexData>( mesh, hint );
}

// ------------------------------------------------------------------------------------------------
VertexData::VertexData( const GPUBufferObject::UsageHint& hint )
{
    _vao = VertexArray::New( hint );
}

// ------------------------------------------------------------------------------------------------
VertexData::VertexData( const std::vector<Math::vec3>& points, const GPUBufferObject::UsageHint& hint )
{
    _vao = VertexArray::New( hint );
    setVertices( points );
}

// ------------------------------------------------------------------------------------------------
VertexData::VertexData( const s2::MeshData3D& meshData, const GPUBufferObject::UsageHint& hint )
{
    _vao = VertexArray::New( hint );

    // Convert double to float vectors
    setVertices( s2::vector_cast<Math::dvec3, Math::vec3>( meshData.vertices ) );
    setNormals( s2::vector_cast<Math::dvec3, Math::vec3>( meshData.normals ) );
    setTextureCoords( s2::vector_cast<Math::dvec2, Math::vec2>( meshData.uvCoords ) );
    setIndices( meshData.indices );
}

// ------------------------------------------------------------------------------------------------
// Standard attributes (DSA)
// ------------------------------------------------------------------------------------------------

void VertexData::setVertices( const std::vector<Math::fvec3>& points )
{
    detail::setAttribute( _vao, VertexAttributeLocation::VA_Position, points );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setVertices( const std::vector<Math::fvec2>& points2D )
{
    detail::setAttribute( _vao, VertexAttributeLocation::VA_Position, points2D );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setColors( const std::vector<Color>& colors )
{
    detail::setAttribute( _vao, VertexAttributeLocation::VA_Color, colors );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setNormals( const std::vector<Math::fvec3>& normals )
{
    detail::setAttribute( _vao, VertexAttributeLocation::VA_Normal, normals );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setTextureCoords( const std::vector<Math::fvec2>& texCoords )
{
    detail::setAttribute( _vao, VertexAttributeLocation::VA_UVCoords, texCoords );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setIndices( const std::vector<uint32_t>& indices )
{
    const uint32_t numIndices = static_cast<uint32_t>( indices.size() );

    if( numIndices == 0 )
        return;

    const int64_t bufferSize = numIndices * sizeof( uint32_t );

    // Create GPU buffer for indices (DSA)
    auto indexGPUBuffer = GPUBufferObject::New(
        bufferSize,
        GPUBufferObject::Type::ElementBuffer,
        _vao->usageHint()
    );

    // Upload index data (DSA)
    indexGPUBuffer->setData( indices.data(), bufferSize, 0 );

    // Create index buffer
    IndexBuffer idxBuf(
        bufferSize,
        IndexBuffer::IndexDataType::UnsignedInt,
        _vao->usageHint()
    );
    idxBuf.setIndices( indices.data(), static_cast<int>( numIndices ) );

    // Set index buffer using DSA (no VAO binding)
    _vao->setIndexBuffer( idxBuf );
}

// ------------------------------------------------------------------------------------------------
// Custom attributes (DSA)
// ------------------------------------------------------------------------------------------------

void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<uint32_t>& attrib )
{
    detail::setAttribute( _vao, loc, attrib );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec2>& attrib )
{
    detail::setAttribute( _vao, loc, attrib );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec3>& attrib )
{
    detail::setAttribute( _vao, loc, attrib );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::ivec4>& attrib )
{
    detail::setAttribute( _vao, loc, attrib );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<float>& attrib )
{
    detail::setAttribute( _vao, loc, attrib );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec2>& attrib )
{
    detail::setAttribute( _vao, loc, attrib );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec3>& attrib )
{
    detail::setAttribute( _vao, loc, attrib );
}

// ------------------------------------------------------------------------------------------------
void VertexData::setAttribute( VertexAttributeLocation loc, const std::vector<Math::fvec4>& attrib )
{
    detail::setAttribute( _vao, loc, attrib );
}

// ------------------------------------------------------------------------------------------------
// Queries
// ------------------------------------------------------------------------------------------------

size_t VertexData::vertexCount() const
{
    const auto& posAttr = _vao->attribute( VertexAttributeLocation::VA_Position );
    return posAttr.isValid() ? posAttr.numberOfVertices() : 0;
}

// ------------------------------------------------------------------------------------------------
size_t VertexData::indexCount() const
{
    const auto& idxBuf = _vao->indexBuffer();
    return idxBuf.isValid() ? idxBuf.count() : 0;
}

}
}