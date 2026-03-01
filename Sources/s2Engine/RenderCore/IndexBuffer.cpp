// IndexBuffer.cpp
// 
#include "IndexBuffer.h"

#include "OpenGLWrap.h"

#include <cassert>

using namespace s2::RenderCore;

// -------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer( int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint )
{
    set( sizeInBytes, dataType, usageHint );
}

// -------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer( const void* data, int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint )
{
    set( sizeInBytes, dataType, usageHint );
	setData( data, sizeInBytes );
}

// -------------------------------------------------------------------------------------------------
void IndexBuffer::set( int64_t sizeInBytes, IndexDataType dataType, GPUBufferObject::UsageHint usageHint )
{
    _gpuBuffer = GPUBufferObject::New( sizeInBytes, GPUBufferObject::Type::ElementBuffer, usageHint );  
    _dataType = dataType;
    updateCount();
}

// -------------------------------------------------------------------------------------------------
void IndexBuffer::updateCount()
{
    if( !_gpuBuffer || !_gpuBuffer->isValid() )
    {
        _count = 0;
        return;
    }

    const int64_t indexSize = (_dataType == IndexDataType::UnsignedInt) 
        ? sizeof(uint32_t) 
        : sizeof(uint16_t);
    
    _count = static_cast<int>( _gpuBuffer->size() / indexSize );
}

// -------------------------------------------------------------------------------------------------
void IndexBuffer::setData( const void* data, int64_t size, int64_t offset )
{
    //assert( isValid() && "IndexBuffer must be valid before setting data" );
    assert( data && "Index data cannot be null" );
    
    _gpuBuffer->setData( data, size, offset );
    updateCount();
}

// -------------------------------------------------------------------------------------------------
void IndexBuffer::setIndices( const uint16_t* indices, int count )
{
    assert( _dataType == IndexDataType::UnsignedShort && "Data type mismatch" );
    setData( indices, count * sizeof(uint16_t) );
}

// -------------------------------------------------------------------------------------------------
void IndexBuffer::setIndices( const uint32_t* indices, int count )
{
    assert( _dataType == IndexDataType::UnsignedInt && "Data type mismatch" );
    setData( indices, count * sizeof(uint32_t) );
}

// -------------------------------------------------------------------------------------------------
void IndexBuffer::getData( void* data, int64_t size, int64_t offset ) const
{
    //assert( isValid() && "IndexBuffer must be valid before getting data" );
    assert( data && "Data buffer cannot be null" );
    
    _gpuBuffer->getData( data, size, offset );
}

// -------------------------------------------------------------------------------------------------
void* IndexBuffer::map( uint32_t accessFlags )
{
    //assert( isValid() && "IndexBuffer must be valid before mapping" );
    
    return _gpuBuffer->mapRange( 0, _gpuBuffer->size(), accessFlags );
}

// -------------------------------------------------------------------------------------------------
bool IndexBuffer::unmap()
{
    //assert( isValid() && "IndexBuffer must be valid before unmapping" );
    
    return _gpuBuffer->unmap();
}
