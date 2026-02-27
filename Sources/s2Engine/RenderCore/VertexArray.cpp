// VertexArray.cpp
// 
#include "VertexArray.h"

#include "Math/Math.h"
#include "Device.h"
#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "OpenGLCheck.h"

#include <cassert>

using namespace s2::RenderCore;

// -------------------------------------------------------------------------------------------------
VertexArrayPtr VertexArray::New( GPUBufferObject::UsageHint hint )
{
    return std::make_shared<VertexArray>( hint );
}

// -------------------------------------------------------------------------------------------------
VertexArray::VertexArray( GPUBufferObject::UsageHint hint )
    : _usageHint( hint )
{
    create();
}

// -------------------------------------------------------------------------------------------------
VertexArray::~VertexArray()
{
    destroy();
}

// -------------------------------------------------------------------------------------------------
void VertexArray::reset()
{
    OpenGLObject::reset();
    _attributes.clear();
    _indexBuffer = std::nullopt;
}

// -------------------------------------------------------------------------------------------------
int VertexArray::objectLabelIdentifier() const
{
    return GL_VERTEX_ARRAY;
}

// -------------------------------------------------------------------------------------------------
void VertexArray::create()
{
    if( isCreated() )
        return;

    destroy();
    OpenGLObject::create();

    const int maxVertexAttrib = Device::maxAttribPerVertex();
    _attributes.resize( maxVertexAttrib );

    // DSA: glCreateVertexArrays (OpenGL 4.5+)
    glCreateVertexArrays( 1, &_objectID );
    glCheck;

    _created = (_objectID != 0);
}

// -------------------------------------------------------------------------------------------------
void VertexArray::destroy()
{
    if( !isCreated() )
        return;

    glDeleteVertexArrays( 1, &_objectID );
    glCheck;
    
    reset();
}

// -------------------------------------------------------------------------------------------------
void VertexArray::bind() const
{
    if( !isCreated() )
        return;

    assert( _objectID != 0 && "VAO must be valid" );

    // DSA: VAO binding is ONLY required for rendering (glDraw* calls)
    // All configuration is done via DSA, so bind is lightweight
    glBindVertexArray( _objectID );
    glCheck;
}

// -------------------------------------------------------------------------------------------------
void VertexArray::unbind() const
{
    glBindVertexArray( 0 );
    glCheck;
}

// -------------------------------------------------------------------------------------------------
const AttributeBuffer& VertexArray::attribute( int index ) const
{
    assert( index >= 0 && index < static_cast<int>( _attributes.size() ) );
    return _attributes[index];
}

// -------------------------------------------------------------------------------------------------
// DSA: Set attribute without VAO binding (OpenGL 4.5+)
// -------------------------------------------------------------------------------------------------
void VertexArray::setAttribute( int location, const AttributeBuffer& attribute )
{
    assert( isCreated() && "VertexArray must be created before setting attributes" );
    assert( location >= 0 && location < static_cast<int>( _attributes.size() ) );
    assert( attribute.isValid() && "AttributeBuffer must be valid" );

    // Store attribute
    _attributes[location] = attribute;

    // Use DSA to attach attribute to VAO (no binding needed)
    const_cast<AttributeBuffer&>( _attributes[location] ).attach( _objectID, location );
}

// -------------------------------------------------------------------------------------------------
const IndexBuffer& VertexArray::indexBuffer() const
{
    return _indexBuffer.value();
}

// -------------------------------------------------------------------------------------------------
// DSA: Set index buffer without VAO binding (OpenGL 4.5+)
// -------------------------------------------------------------------------------------------------
void VertexArray::setIndexBuffer( const IndexBuffer& indexBuffer )
{
    assert( isCreated() && "VertexArray must be created before setting index buffer" );
    //assert( indexBuffer.isValid() && "IndexBuffer must be valid" );

    // Store index buffer
    _indexBuffer = indexBuffer;

    // DSA: Bind element buffer to VAO (OpenGL 4.5+)
    glVertexArrayElementBuffer( _objectID, indexBuffer.id() );
    glCheck;
}

// -------------------------------------------------------------------------------------------------
bool VertexArray::isIndexed() const
{
    return _indexBuffer.has_value();
}

// -------------------------------------------------------------------------------------------------
int VertexArray::maxArrayIndex() const
{
    int maximumArrayIndex = 0;

    for( const auto& attr : _attributes )
    {
        if( attr.isValid() )
            maximumArrayIndex = Math::max( attr.numberOfVertices() - 1, maximumArrayIndex );
    }

    return maximumArrayIndex;
}

// -------------------------------------------------------------------------------------------------
int VertexArray::attributeCount() const
{
    int count = 0;
    for( const auto& attr : _attributes )
    {
        if( attr.isValid() )
            ++count;
    }
    return count;
}