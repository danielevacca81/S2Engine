// IndexBuffer.cpp
// 
#include "IndexBuffer.h"

#include "OpenGL/OpenGLWrap.h"

using namespace OpenGL;

//-------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer()
: _count(0)
, _bufferObject(nullptr)
, _valid(false)
{}

//-------------------------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{}

//-------------------------------------------------------------------------------------------------
void IndexBuffer::set( int sizeInBytes, IndexDataType dataType, BufferObject::BufferUsageHint usageHint )
{
	_bufferObject = BufferObject::New( sizeInBytes, BufferObject::ElementBuffer, usageHint );
	_valid        = true;
	_dataType     = dataType;
}

//-------------------------------------------------------------------------------------------------
void IndexBuffer::bind() { if( _valid ) _bufferObject->bind(); }

//-------------------------------------------------------------------------------------------------
void IndexBuffer::unbind() { if( _valid ) _bufferObject->unbind(); }

//-------------------------------------------------------------------------------------------------
void IndexBuffer::sendData( void *data, int length, int offset )
{
	if( _valid )
	{
		//_count = _bufferObject->size() / SizeInBytes<T>.Value;
		_bufferObject->sendData( data, length, offset );
	}
}

//-------------------------------------------------------------------------------------------------
void * IndexBuffer::receiveData( int length, int offset )
{ 
	if( !_valid )
		return 0;

	return _bufferObject->receiveData( length, offset ); 
}

//-------------------------------------------------------------------------------------------------
void * IndexBuffer::mapData( BufferObject::BufferMapMode mode )
{
	if( !_valid )
		return 0;
	
	return _bufferObject->mapData( mode ); 
}

//-------------------------------------------------------------------------------------------------
bool IndexBuffer::unmapData()
{
	if( !_valid )
		return false;
	
	return _bufferObject->unmapData(); 
}