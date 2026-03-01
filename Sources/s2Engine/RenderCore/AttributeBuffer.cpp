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
    int64_t offset,
    int64_t stride)
{
    _gpuBuffer          = GPUBufferObject::New( bufferSize, bufferType, usageHint );
    _componentDatatype  = componentDatatype;
    _numberOfComponents = numberOfComponents;
    _offset             = offset;
    _normalize          = normalize;
    _stride             = stride == 0 
        ? numberOfComponents * dataTypeSize( componentDatatype ) // tightly packed
        : stride;

    if( data && bufferSize > 0 )
        _gpuBuffer->setData( data, bufferSize );
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
// DSA: Attach attribute to VAO (OpenGL 4.5+)
// -------------------------------------------------------------------------------------------------
void AttributeBuffer::attach( unsigned int vaoID, int location )
{
    //assert( _valid && "AttributeBuffer must be valid before attaching" );
    assert( vaoID != 0 && "VAO ID must be valid" );
    assert(  _gpuBuffer && _gpuBuffer->isValid() && "GPUBufferObject must be valid before attaching" );

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
