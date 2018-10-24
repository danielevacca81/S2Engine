// IndexBuffer.cpp
// 
#include "IndexBuffer.h"

#include "OpenGLWrap.h"

using namespace s2;
using namespace s2::OpenGL;

// -------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer()
: _count(0)
, _valid(false)
{}

// -------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer( IndexBuffer &&other )
{
	std::swap( _count,        other._count );
	std::swap( _valid,        other._valid);
	std::swap( _dataType,     other._dataType);
	std::swap( _bufferObject, other._bufferObject );
}

// -------------------------------------------------------------------------------------------------
//IndexBuffer::IndexBuffer( int sizeInBytes, const IndexDataType &dataType, const BufferObject::UsageHint &usageHint )
//{
//	set( sizeInBytes, dataType, usageHint );
//}

// -------------------------------------------------------------------------------------------------
IndexBuffer &IndexBuffer::operator=( IndexBuffer &&other )
{
	std::swap( _count,        other._count );
	std::swap( _valid,        other._valid);
	std::swap( _dataType,     other._dataType);
	std::swap( _bufferObject, other._bufferObject );
	return *this;
}

//-------------------------------------------------------------------------------------------------
void IndexBuffer::set( int sizeInBytes, const IndexDataType &dataType, const BufferObject::UsageHint &usageHint )
{
	_bufferObject = BufferObject( sizeInBytes, BufferObject::Type::ElementBuffer, usageHint );
	_valid        = true;
	_dataType     = dataType;
}

//-------------------------------------------------------------------------------------------------
void IndexBuffer::bind() { if( _valid ) _bufferObject.bind(); }

//-------------------------------------------------------------------------------------------------
void IndexBuffer::unbind() { if( _valid ) _bufferObject.unbind(); }

//-------------------------------------------------------------------------------------------------
void IndexBuffer::sendData( void *data, int length, int offset )
{
	if( _valid )
	{
		_count = _bufferObject.size() / ( _dataType == UnsignedInt ? sizeof( unsigned int) : sizeof(unsigned short) );
		_bufferObject.sendData( data, length, offset );
	}
}

//-------------------------------------------------------------------------------------------------
void * IndexBuffer::receiveData( int length, int offset )
{ 
	if( !_valid )
		return 0;

	return _bufferObject.receiveData( length, offset ); 
}

//-------------------------------------------------------------------------------------------------
void * IndexBuffer::mapData(const BufferObject::MapMode &mode )
{
	if( !_valid )
		return 0;
	
	return _bufferObject.mapData( mode ); 
}

//-------------------------------------------------------------------------------------------------
bool IndexBuffer::unmapData()
{
	if( !_valid )
		return false;
	
	return _bufferObject.unmapData(); 
}