// AttributeBuffer.cpp
// 
#include "AttributeBuffer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"

#include <cassert>

using namespace s2::RenderCore;

// -------------------------------------------------------------------------------------------------
static inline int64_t dataTypeSize( AttributeBuffer::ComponentDatatype type )
{
    switch( type )
    {
    case AttributeBuffer::ComponentDatatype::Byte:
    case AttributeBuffer::ComponentDatatype::UnsignedByte:  return sizeof( int8_t );

    case AttributeBuffer::ComponentDatatype::Short:         return sizeof( int16_t );
    case AttributeBuffer::ComponentDatatype::UnsignedShort: return sizeof( uint16_t );
    
    case AttributeBuffer::ComponentDatatype::Int:           return sizeof( int32_t );
    case AttributeBuffer::ComponentDatatype::UnsignedInt:   return sizeof( uint32_t );
    
    case AttributeBuffer::ComponentDatatype::Float:         return sizeof( float );    
    case AttributeBuffer::ComponentDatatype::HalfFloat:     return sizeof( int16_t ); // 16-bit float
    
    case AttributeBuffer::ComponentDatatype::Double:        return sizeof( double );
    }

    return sizeof( int8_t );
}

// -------------------------------------------------------------------------------------------------
AttributeBuffer::AttributeBuffer(
    const void *data,
    int64_t bufferSize,
    GPUBufferObject::Type bufferType,
    GPUBufferObject::UsageHint usageHint,
    ComponentDatatype componentDatatype,
    int numberOfComponents,
    bool normalize,
    int64_t bufferOffset,
    int64_t stride)
{
    _gpuBuffer          = GPUBufferObject::New( bufferSize, bufferType, usageHint );
    _componentDatatype  = componentDatatype;
    _numberOfComponents = numberOfComponents;
    _bufferOffset       = bufferOffset;
    _relativeOffset     = 0; // separate layout — no field offset within vertex
    _normalize          = normalize;
    _stride             = stride == 0 
        ? numberOfComponents * dataTypeSize( componentDatatype ) // tightly packed
        : stride;

    if( data && bufferSize > 0 )
        _gpuBuffer->setData( data, bufferSize );
}

// -------------------------------------------------------------------------------------------------
AttributeBuffer::AttributeBuffer( const GPUBufferObjectPtr& gpuBuffer,
                                  ComponentDatatype componentDatatype,
                                  int numberOfComponents,
                                  bool normalize,
                                  int64_t bufferOffset,
                                  int64_t relativeOffset,
                                  int64_t stride )
{
    _gpuBuffer          = gpuBuffer;
    _componentDatatype  = componentDatatype;
    _numberOfComponents = numberOfComponents;
    _bufferOffset       = bufferOffset;
    _relativeOffset     = relativeOffset;
    _normalize          = normalize;
    _stride             = stride == 0 
        ? numberOfComponents * dataTypeSize( componentDatatype ) // tightly packed
        : stride;
}   

// -------------------------------------------------------------------------------------------------
void AttributeBuffer::setObjectLabel( const std::string& label )
{
    if( _gpuBuffer ) // can be null if default constructor was used
        _gpuBuffer->setObjectLabel( label );
}

// -------------------------------------------------------------------------------------------------
int AttributeBuffer::numberOfVertices() const
{
    if( _stride == 0 || !_gpuBuffer || _gpuBuffer->size() == 0 )
        return 0;

    return static_cast<int>( _gpuBuffer->size() / _stride );
}

// -------------------------------------------------------------------------------------------------
void AttributeBuffer::setData( const void* data, int64_t size )
{
    assert( _gpuBuffer && _gpuBuffer->isValid() && "GPUBufferObject must be valid" );
    assert( size <= _gpuBuffer->size() && "Data size exceeds buffer capacity" );
    _gpuBuffer->setData( data, size );
}

// -------------------------------------------------------------------------------------------------
void AttributeBuffer::setSubData( const void* data, int64_t size, int64_t offset )
{
    assert( _gpuBuffer && _gpuBuffer->isValid() && "GPUBufferObject must be valid" );
    assert( offset + size <= _gpuBuffer->size() && "Sub-data range exceeds buffer capacity" );
    _gpuBuffer->setData( data, size, offset );
}

// -------------------------------------------------------------------------------------------------
void* AttributeBuffer::map( uint32_t accessFlags )
{
    assert( _gpuBuffer && _gpuBuffer->isValid() && "GPUBufferObject must be valid" );
    return _gpuBuffer->mapRange( 0, _gpuBuffer->size(), accessFlags );
}

// -------------------------------------------------------------------------------------------------
void* AttributeBuffer::mapRange( int64_t offset, int64_t length, uint32_t accessFlags )
{
    assert( _gpuBuffer && _gpuBuffer->isValid() && "GPUBufferObject must be valid" );
    return _gpuBuffer->mapRange( offset, length, accessFlags );
}

// -------------------------------------------------------------------------------------------------
bool AttributeBuffer::unmap()
{
    assert( _gpuBuffer && _gpuBuffer->isValid() && "GPUBufferObject must be valid" );
    return _gpuBuffer->unmap();
}

// -------------------------------------------------------------------------------------------------
// DSA: Attach attribute to VAO (OpenGL 4.5+)
//
// OpenGL 4.5 separates vertex format from buffer binding:
//
//   glVertexArrayVertexBuffer(vao, bindingIndex, buffer, bufferOffset, stride)
//     -> which buffer supplies vertex data, and where in it.
//
//   glVertexArrayAttribFormat(vao, attribIndex, size, type, normalized, relativeOffset)
//     -> how to interpret bytes within each vertex.
//
//   glVertexArrayAttribBinding(vao, attribIndex, bindingIndex)
//     -> which binding point feeds this attribute.
//
// For interleaved data, multiple attributes can share the same binding point
// (same buffer, same bufferOffset, same stride) and differ only in relativeOffset.
// The caller controls this via VertexArray::setAttribute with an explicit bindingIndex,
// or by default each attribute gets its own binding point (bindingIndex = location).
// -------------------------------------------------------------------------------------------------
void AttributeBuffer::attach( unsigned int vaoID, int location )
{
    assert( vaoID != 0 && "VAO ID must be valid" );
    assert( _gpuBuffer && _gpuBuffer->isValid() && "GPUBufferObject must be valid before attaching" );

    _location = location;

    // Enable attribute array (DSA)
    glEnableVertexArrayAttrib( vaoID, location );
    glCheck;

    // Bind VBO to VAO's binding point (DSA)
    // bufferOffset = byte offset to the first vertex in the GPU buffer
    glVertexArrayVertexBuffer( 
        vaoID, 
        location,                                // binding index (= location by default)
        _gpuBuffer->id(),                        // buffer ID
        _bufferOffset,                           // buffer-level offset
        static_cast<GLsizei>( _stride )          // stride between consecutive vertices
    );
    glCheck;

    // Set attribute format (DSA)
    // relativeOffset = byte offset of this field within each vertex
    const GLenum glType       = glWrap( _componentDatatype );
    const GLuint relOffset    = static_cast<GLuint>( _relativeOffset );

    switch( _componentDatatype )
    {
    // Integer types — use AttribIFormat ONLY when normalization is NOT requested.
    // When normalize is true, we must use AttribFormat so the driver converts
    // integer values to [0,1] / [-1,1] floats before they reach the shader.
    case ComponentDatatype::Byte:
    case ComponentDatatype::UnsignedByte:
    case ComponentDatatype::Short:
    case ComponentDatatype::UnsignedShort:
    case ComponentDatatype::Int:
    case ComponentDatatype::UnsignedInt:
        if( _normalize )
        {
            glVertexArrayAttribFormat(
                vaoID,
                location,
                _numberOfComponents,
                glType,
                GL_TRUE,
                relOffset
            );
        }
        else
        {
            glVertexArrayAttribIFormat(
                vaoID,
                location,
                _numberOfComponents,
                glType,
                relOffset
            );
        }
        glCheck;
        break;

    // Double precision types (glVertexArrayAttribLFormat)
    case ComponentDatatype::Double:
        glVertexArrayAttribLFormat(
            vaoID,
            location,
            _numberOfComponents,
            glType,
            relOffset
        );
        glCheck;
        break;

    // Floating point types (glVertexArrayAttribFormat)
    case ComponentDatatype::Float:
    case ComponentDatatype::HalfFloat:
    default:
        glVertexArrayAttribFormat(
            vaoID,
            location,
            _numberOfComponents,
            glType,
            _normalize ? GL_TRUE : GL_FALSE,
            relOffset
        );
        glCheck;
        break;
    }

    // Bind attribute to binding point (DSA)
    glVertexArrayAttribBinding( vaoID, location, location );
    glCheck;
}

// -------------------------------------------------------------------------------------------------
// DSA: Detach attribute from VAO (OpenGL 4.5+)
// -------------------------------------------------------------------------------------------------
void AttributeBuffer::detach( unsigned int vaoID )
{
    if( _location < 0 )
        return;

    assert( vaoID != 0 && "VAO ID must be valid" );

    // Disable attribute array (DSA)
    glDisableVertexArrayAttrib( vaoID, _location );
    glCheck;

    _location = -1;
}
