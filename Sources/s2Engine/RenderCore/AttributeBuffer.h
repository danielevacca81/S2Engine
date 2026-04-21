// AttributeBuffer.h
// 
#ifndef S2_RENDERCORE_ATTRIBUTEBUFFER_H
#define S2_RENDERCORE_ATTRIBUTEBUFFER_H

#include "GPUBufferObject.h"

#include "s2Engine_API.h"

#include <cstdint>
#include <string>
#include <span>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                     AttributeBuffer                                          */
/************************************************************************************************/
class S2ENGINE_API AttributeBuffer
{
public:
    enum class ComponentDatatype
    {
        Byte,
        UnsignedByte,
        Short,
        UnsignedShort,
        Int,
        UnsignedInt,
        Float,
        HalfFloat,
        Double,
    };

public:
    AttributeBuffer() = default;

    // Allocates a new GPU buffer and fills it with data (separate layout)
    AttributeBuffer(
        const void *data,
        int64_t bufferSize,
        GPUBufferObject::Type bufferType,
        GPUBufferObject::UsageHint usageHint,
        ComponentDatatype componentDatatype,
        int numberOfComponents,
        bool normalize = false,
        int64_t bufferOffset = 0,
        int64_t stride = 0 );

    // Wraps an existing GPU buffer (supports both separate and interleaved layouts)
    //
    //   bufferOffset   - byte offset from the start of the GPU buffer to the first vertex.
    //                     Maps to glVertexArrayVertexBuffer(..., offset, ...).
    //
    //   relativeOffset - byte offset from the start of each vertex to this attribute's data.
    //                     Maps to glVertexArrayAttribFormat(..., relativeOffset).
    //                     For separate (non-interleaved) buffers this is 0.
    //                     For interleaved buffers use offsetof(Vertex, field).
    //
    //   stride         - byte distance between consecutive vertices.
    //                     0 = tightly packed (computed from componentDatatype × numberOfComponents).
    //                     For interleaved buffers use sizeof(Vertex).
    //
    AttributeBuffer( const GPUBufferObjectPtr& gpuBuffer,
                     ComponentDatatype componentDatatype,
                     int numberOfComponents,
                     bool normalize = false,
                     int64_t bufferOffset = 0,
                     int64_t relativeOffset = 0,
                     int64_t stride = 0 );

    void attach( unsigned int vaoID, int location );  // Attach to VAO using DSA (requires VAO ID)
    void detach( unsigned int vaoID );                // Detach from VAO using DSA (requires VAO ID)

    // ===== Data Update API =====

    // Replace entire buffer data (same size)
    void setData( const void* data, int64_t size );

    // Update a sub-region of the buffer
    void setSubData( const void* data, int64_t size, int64_t offset );

    // Type-safe update from a typed span (e.g. std::span<glm::vec3>)
    template<typename T>
    void setData( std::span<const T> elements )
    {
        setData( elements.data(), static_cast<int64_t>( elements.size_bytes() ) );
    }

    // Type-safe partial update from a typed span
    template<typename T>
    void setSubData( std::span<const T> elements, int64_t offsetInBytes = 0 )
    {
        setSubData( elements.data(), static_cast<int64_t>( elements.size_bytes() ), offsetInBytes );
    }

    // Map/unmap for direct GPU memory access
    void* map( uint32_t accessFlags );
    void* mapRange( int64_t offset, int64_t length, uint32_t accessFlags );
    bool  unmap();

    // ===== Queries =====

    ComponentDatatype componentDatatype()  const { return _componentDatatype; }
    bool              isValid()            const { return _gpuBuffer && _gpuBuffer->isValid(); }
    int               numberOfComponents() const { return _numberOfComponents; }
    int64_t           bufferOffset()       const { return _bufferOffset; }
    int64_t           relativeOffset()     const { return _relativeOffset; }
    bool              normalize()          const { return _normalize; }
    int64_t           strideInBytes()      const { return _stride; }
    int               numberOfVertices()   const;
    int               location()           const { return _location; }
    int64_t           sizeInBytes()        const { return _gpuBuffer ? _gpuBuffer->size() : 0; }

    // returns bufferOffset for backward compatibility
    int64_t           offset()             const { return _bufferOffset; }

    //
    void setObjectLabel( const std::string& label );

private:
    GPUBufferObjectPtr _gpuBuffer;
    ComponentDatatype  _componentDatatype { ComponentDatatype::Float };

    int     _location           { -1 };
    int     _numberOfComponents { 0 };
    int64_t _bufferOffset       { 0 };   // byte offset into the GPU buffer (glVertexArrayVertexBuffer)
    int64_t _relativeOffset     { 0 };   // byte offset within a vertex (glVertexArrayAttribFormat)
    int64_t _stride             { 0 };
    bool    _normalize          { false };
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_ATTRIBUTEBUFFER_H