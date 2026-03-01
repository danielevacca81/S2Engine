// GPUBufferObject.cpp
//
#include "GPUBufferObject.h"


#include "OpenGL.h"
#include "OpenGLCheck.h"
#include "OpenGLWrap.h"

#include <cassert>
#include <cstring>

using namespace s2::RenderCore;

// ------------------------------------------------------------------------------------------------
GPUBufferObjectPtr GPUBufferObject::New( int64_t sizeInBytes, Type type, UsageHint usageHint )
{
    return std::make_shared<GPUBufferObject>( sizeInBytes, type, usageHint );
}

// ------------------------------------------------------------------------------------------------
GPUBufferObjectPtr GPUBufferObject::NewImmutable( int64_t sizeInBytes, Type type, uint32_t storageFlags, const void* initialData )
{
    auto buffer = std::make_shared<GPUBufferObject>( sizeInBytes, type, UsageHint::StaticDraw );
    buffer->_immutableStorage = true;
    buffer->_storageFlags = storageFlags;
    
    buffer->create();
    buffer->allocateImmutableStorage( storageFlags, initialData );
    
    return buffer;
}

// ------------------------------------------------------------------------------------------------
GPUBufferObject::GPUBufferObject( int64_t sizeInBytes, Type type, UsageHint usageHint )
    : _size( sizeInBytes )
    , _type( type )
    , _usageHint( usageHint )
    , _immutableStorage( false )
    , _storageFlags( 0 )
    , _gpuAddress( 0 )
    , _resident( false )
{
    create();
}

// ------------------------------------------------------------------------------------------------
GPUBufferObject::~GPUBufferObject()
{
    destroy();
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::create()
{
    if( isValid() )
        return;

    OpenGLObject::create();

    // DSA: glCreateBuffers (OpenGL 4.5+)
    glCreateBuffers( 1, &_objectID );
    glCheck;

    if( _objectID == 0 )
    {
        assert( false && "Failed to create buffer object" );
        return;
    }

    // Allocate storage if not immutable
    if( !_immutableStorage )
        allocateStorage( nullptr );
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::destroy()
{
    if( !isValid() )
        return;

    // Make non-resident if bindless
    if( _resident )
        makeNonResident();

    glDeleteBuffers( 1, &_objectID );
    glCheck;

    reset();
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::reset()
{
    OpenGLObject::reset();
    _gpuAddress = 0;
    _resident = false;
    _immutableStorage = false;
    _storageFlags = 0;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::allocateStorage( const void* initialData )
{
    assert( isValid() && "Buffer must be created before allocating storage" );
    assert( !_immutableStorage && "Cannot reallocate immutable storage" );

    const GLenum usage = glWrap( _usageHint );

    // DSA: glNamedBufferData (OpenGL 4.5+)
    glNamedBufferData( _objectID, _size, initialData, usage );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::allocateImmutableStorage( uint32_t storageFlags, const void* initialData )
{
    assert( isValid() && "Buffer must be created before allocating storage" );

    // DSA: glNamedBufferStorage (OpenGL 4.4+)
    glNamedBufferStorage( _objectID, _size, initialData, storageFlags );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::setData( const void* data, int64_t size, int64_t offset )
{
    assert( isValid() && "Buffer must be created before setting data" );
    assert( data && "Data cannot be null" );
    assert( offset + size <= _size && "Data exceeds buffer size" );

    // DSA: glNamedBufferSubData (OpenGL 4.5+)
    glNamedBufferSubData( _objectID, offset, size, data );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::getData( void* data, int64_t size, int64_t offset ) const
{
    assert( isValid() && "Buffer must be created before getting data" );
    assert( data && "Data cannot be null" );
    assert( offset + size <= _size && "Data exceeds buffer size" );

    // DSA: glGetNamedBufferSubData (OpenGL 4.5+)
    glGetNamedBufferSubData( _objectID, offset, size, data );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void* GPUBufferObject::mapRange( int64_t offset, int64_t length, uint32_t accessFlags )
{
    assert( isValid() && "Buffer must be created before mapping" );
    assert( offset + length <= _size && "Map range exceeds buffer size" );

    // DSA: glMapNamedBufferRange (OpenGL 4.5+)
    void* ptr = glMapNamedBufferRange( _objectID, offset, length, accessFlags );
    glCheck;

    return ptr;
}

// ------------------------------------------------------------------------------------------------
bool GPUBufferObject::unmap()
{
    assert( isValid() && "Buffer must be created before unmapping" );

    // DSA: glUnmapNamedBuffer (OpenGL 4.5+)
    GLboolean result = glUnmapNamedBuffer( _objectID );
    glCheck;

    return result == GL_TRUE;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::flushMappedRange( int64_t offset, int64_t length )
{
    assert( isValid() && "Buffer must be created before flushing" );

    // DSA: glFlushMappedNamedBufferRange (OpenGL 4.5+)
    glFlushMappedNamedBufferRange( _objectID, offset, length );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::copyTo( 
    const GPUBufferObjectPtr& destination, 
    int64_t readOffset, 
    int64_t writeOffset, 
    int64_t size ) const
{
    assert( isValid() && "Source buffer must be created" );
    assert( destination && destination->isValid() && "Destination buffer must be created" );
    assert( readOffset + size <= _size && "Read range exceeds source buffer size" );
    assert( writeOffset + size <= destination->size() && "Write range exceeds destination buffer size" );

    // DSA: glCopyNamedBufferSubData (OpenGL 4.5+)
    glCopyNamedBufferSubData( 
        _objectID, 
        destination->id(), 
        readOffset, 
        writeOffset, 
        size 
    );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::clear( const void* clearValue )
{
    assert( isValid() && "Buffer must be created before clearing" );

    // DSA: glClearNamedBufferData (OpenGL 4.3+)
    glClearNamedBufferData( 
        _objectID, 
        GL_R8UI,              // Internal format
        GL_RED_INTEGER,       // Format
        GL_UNSIGNED_BYTE,     // Type
        clearValue 
    );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::invalidate()
{
    assert( isValid() && "Buffer must be created before invalidating" );

    // DSA: glInvalidateBufferData (OpenGL 4.3+)
    glInvalidateBufferData( _objectID );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::invalidateRange( int64_t offset, int64_t length )
{
    assert( isValid() && "Buffer must be created before invalidating" );
    assert( offset + length <= _size && "Range exceeds buffer size" );

    // DSA: glInvalidateBufferSubData (OpenGL 4.3+)
    glInvalidateBufferSubData( _objectID, offset, length );
    glCheck;
}

// ------------------------------------------------------------------------------------------------
uint64_t GPUBufferObject::getGPUAddress() const
{
    assert( isValid() && "Buffer must be created before getting GPU address" );

    if( _gpuAddress == 0 )
    {
        // ARB_gpu_shader_int64 / NV_shader_buffer_load
        glGetNamedBufferParameterui64vNV( _objectID, GL_BUFFER_GPU_ADDRESS_NV, &_gpuAddress );
        glCheck;
    }

    return _gpuAddress;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::makeResident( uint32_t access )
{
    if( _resident )
        return;

    // NV_shader_buffer_load
    glMakeNamedBufferResidentNV( _objectID, access );
    glCheck;

    _resident = true;
}

// ------------------------------------------------------------------------------------------------
void GPUBufferObject::makeNonResident()
{
    if( !_resident )
        return;

    // NV_shader_buffer_load
    glMakeNamedBufferNonResidentNV( _objectID );
    glCheck;

    _resident = false;
}

// ------------------------------------------------------------------------------------------------
int GPUBufferObject::objectLabelIdentifier() const
{
    return GL_BUFFER;
}
