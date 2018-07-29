// VertexBuffer.cpp
// 
#include "VertexBuffer.h"

#include "OpenGL.h"
#include "OpenGLWrap.h"

using namespace s2::OpenGL;

//-------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer()
: _bufferObject(nullptr)
, _valid(false)
{}

//-------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer( int sizeInBytes, const BufferObject::BufferUsageHint &usageHint )
{
	set( sizeInBytes, usageHint );
}

//-------------------------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{}

//-------------------------------------------------------------------------------------------------
void VertexBuffer::set( int sizeInBytes, const BufferObject::BufferUsageHint &usageHint )
{
	_bufferObject = BufferObject::New( sizeInBytes, BufferObject::ArrayBuffer, usageHint );
	_valid = true;
}

//-------------------------------------------------------------------------------------------------
void VertexBuffer::bind()
{
	if( _valid )
		_bufferObject->bind();
}

//-------------------------------------------------------------------------------------------------
void VertexBuffer::unbind()
{
	if( _valid )
		_bufferObject->unbind();
}

//-------------------------------------------------------------------------------------------------
void VertexBuffer::sendData( void *data, int length, int offset )
{
	if( _valid )
		_bufferObject->sendData( data, length, offset );
}

//-------------------------------------------------------------------------------------------------
void * VertexBuffer::receiveData( int length, int offset )
{
	if( !_valid )
		return nullptr;

	return _bufferObject->receiveData( length,offset );
}

//-------------------------------------------------------------------------------------------------
void * VertexBuffer::mapData( const BufferObject::BufferMapMode &mode )
{
	if( !_valid )
		return nullptr;

	return _bufferObject->mapData( mode );
}

//-------------------------------------------------------------------------------------------------
bool VertexBuffer::unmapData()
{
	if( !_valid )
		return false;

	return _bufferObject->unmapData();
}