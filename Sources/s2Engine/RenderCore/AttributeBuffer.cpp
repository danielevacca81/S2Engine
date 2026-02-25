// AttributeBuffer.cpp
// 
#include "AttributeBuffer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"

#include <cassert>

using namespace s2::RenderCore;

// -------------------------------------------------------------------------------------------------
static int64_t dataTypeSize( AttributeBuffer::ComponentDatatype type )
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
void AttributeBuffer::set( 
    const GPUBufferObjectPtr& buffer,
    ComponentDatatype componentDatatype, 
    int numberOfComponents, 
    bool normalize, 
    int64_t offset, 
    int64_t stride )
{
    _gpuBuffer          = buffer;
    _componentDatatype  = componentDatatype;
    _numberOfComponents = numberOfComponents;
    _offset             = offset;
    _normalize          = normalize;
    _valid              = true;
    
    // Calculate stride: if 0, assume tightly packed
    _stride = (stride == 0) ? calculateStride() : stride;
}

// -------------------------------------------------------------------------------------------------
int64_t AttributeBuffer::calculateStride() const
{
    return _numberOfComponents * dataTypeSize( _componentDatatype );
}

// -------------------------------------------------------------------------------------------------
int AttributeBuffer::numberOfVertices() const
{
    if( !_valid || _stride == 0 )
        return 0;

    return static_cast<int>( _gpuBuffer->size() / _stride );
}

// -------------------------------------------------------------------------------------------------
// DSA: Attach attribute to VAO (OpenGL 4.5+)
// -------------------------------------------------------------------------------------------------
void AttributeBuffer::attach( unsigned int vaoID, int location )
{
    assert( _valid && "AttributeBuffer must be valid before attaching" );
    assert( vaoID != 0 && "VAO ID must be valid" );

    _location = location;

    // Enable attribute array (DSA)
    glEnableVertexArrayAttrib( vaoID, location );
    glCheck;

    // Bind VBO to VAO's binding point (DSA)
    glVertexArrayVertexBuffer( 
        vaoID, 
        location,                        // binding index
        _gpuBuffer->id(),                // buffer ID
        _offset,                         // offset
        static_cast<GLsizei>( _stride )  // stride
    );
    glCheck;

    // Set attribute format based on data type (DSA)
    const GLenum glType = glWrap( _componentDatatype );

    switch( _componentDatatype )
    {
    // Integer types (glVertexArrayAttribIFormat)
    case ComponentDatatype::Byte:
    case ComponentDatatype::UnsignedByte:
    case ComponentDatatype::Short:
    case ComponentDatatype::UnsignedShort:
    case ComponentDatatype::Int:
    case ComponentDatatype::UnsignedInt:
        glVertexArrayAttribIFormat( 
            vaoID,
            location,
            _numberOfComponents,
            glType,
            0  // relative offset (we use buffer offset in glVertexArrayVertexBuffer)
        );
        glCheck;
        break;

    // Double precision types (glVertexArrayAttribLFormat)
    case ComponentDatatype::Double:
        glVertexArrayAttribLFormat(
            vaoID,
            location,
            _numberOfComponents,
            glType,
            0  // relative offset
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
            0  // relative offset
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
