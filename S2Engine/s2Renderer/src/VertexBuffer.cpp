// VertexBuffer.cpp
// 
#include "VertexBuffer.h"

#include "OpenGL.h"


using namespace Renderer;

// -------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer( int sizeInBytes, const BufferObject::UsageHint &usageHint )
{
	set( sizeInBytes, usageHint );
}

// -------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer( void *data, int sizeInBytes, const BufferObject::UsageHint &usageHint )
{
	set( sizeInBytes, usageHint );
	sendData( data, sizeInBytes, 0 );
}


// -------------------------------------------------------------------------------------------------
//VertexBuffer::VertexBuffer( VertexBuffer &&other )
//: VertexBuffer()
//{
//	std::swap( _bufferObject, other._bufferObject );
//}
//
//// -------------------------------------------------------------------------------------------------
//VertexBuffer &VertexBuffer::operator=( VertexBuffer &&other )
//{
//	std::swap( _bufferObject, other._bufferObject );
//	return *this;
//}


// -------------------------------------------------------------------------------------------------
void VertexBuffer::set( int sizeInBytes, const BufferObject::UsageHint &usageHint )
{
	_bufferObject = BufferObject::New( sizeInBytes, BufferObject::Type::ArrayBuffer, usageHint );
}

// -------------------------------------------------------------------------------------------------
void VertexBuffer::bind() const
{
	if( _bufferObject )
		_bufferObject->bind();
}

// -------------------------------------------------------------------------------------------------
void VertexBuffer::unbind() const
{
	if( _bufferObject )
		_bufferObject->unbind();
}

// -------------------------------------------------------------------------------------------------
void VertexBuffer::sendData( void *data, int length, int offset )
{
	if( _bufferObject )
		_bufferObject->sendData( data, length, offset );
}

// -------------------------------------------------------------------------------------------------
void * VertexBuffer::receiveData( int length, int offset )
{
	if( !_bufferObject )
		return nullptr;

	return _bufferObject->receiveData( length,offset );
}

// -------------------------------------------------------------------------------------------------
void * VertexBuffer::mapData( const BufferObject::MapMode &mode )
{
	if( !_bufferObject )
		return nullptr;

	return _bufferObject->mapData( mode );
}

// -------------------------------------------------------------------------------------------------
bool VertexBuffer::unmapData()
{
	if( !_bufferObject )
		return false;

	return _bufferObject->unmapData();
}