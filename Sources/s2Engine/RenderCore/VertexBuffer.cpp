// VertexBuffer.cpp
// 
#include "VertexBuffer.h"

#include "OpenGL.h"


using namespace s2::RenderCore;

// -------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer( int sizeInBytes, const GPUBufferObject::UsageHint &usageHint )
{
	set( sizeInBytes, usageHint );
}

// -------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer( void *data, int sizeInBytes, const GPUBufferObject::UsageHint &usageHint )
{
	set( sizeInBytes, usageHint );
	sendData( data, sizeInBytes, 0 );
}

// -------------------------------------------------------------------------------------------------
void VertexBuffer::set( int sizeInBytes, const GPUBufferObject::UsageHint &usageHint )
{
	_bufferObject = GPUBufferObject::New( sizeInBytes, GPUBufferObject::Type::ArrayBuffer, usageHint );
}

// -------------------------------------------------------------------------------------------------
void VertexBuffer::bind() const
{
	//if( _bufferObject )
		_bufferObject->bind();
}

// -------------------------------------------------------------------------------------------------
void VertexBuffer::unbind() const
{
	//if( _bufferObject )
		_bufferObject->unbind();
}

// -------------------------------------------------------------------------------------------------
void VertexBuffer::sendData( void *data, int length, int offset )
{
	//if( _bufferObject )
		_bufferObject->sendData( data, length, offset );
}

// -------------------------------------------------------------------------------------------------
void * VertexBuffer::receiveData( int length, int offset )
{
	//if( !_bufferObject )
	//	return nullptr;

	return _bufferObject->receiveData( length,offset );
}

// -------------------------------------------------------------------------------------------------
void * VertexBuffer::mapData( const GPUBufferObject::MapMode &mode )
{
	//if( !_bufferObject )
	//	return nullptr;

	return _bufferObject->mapData( mode );
}

// -------------------------------------------------------------------------------------------------
bool VertexBuffer::unmapData()
{
	//if( !_bufferObject )
	//	return false;

	return _bufferObject->unmapData();
}