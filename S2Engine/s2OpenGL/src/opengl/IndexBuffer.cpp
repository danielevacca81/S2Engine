// IndexBuffer.cpp
// 
#include "IndexBuffer.h"

#include "OpenGLWrap.h"

using namespace s2;
using namespace s2::OpenGL;

// -------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer()
: _count(0)
, _bufferObject(nullptr)
, _valid(false)
{}

// -------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer( int sizeInBytes, const IndexDataType &dataType, const BufferObject::BufferUsageHint &usageHint )
{
	set( sizeInBytes, dataType, usageHint );
}

//-------------------------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{}

//-------------------------------------------------------------------------------------------------
void IndexBuffer::set( int sizeInBytes, const IndexDataType &dataType, const BufferObject::BufferUsageHint &usageHint )
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
		_count = _bufferObject->size() / ( _dataType == UnsignedInt ? sizeof( unsigned int) : sizeof(unsigned short) );
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