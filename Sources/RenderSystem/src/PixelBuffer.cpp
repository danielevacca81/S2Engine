// PixelBuffer.cpp
//
#include "PixelBuffer.h"

using namespace RenderSystem;


/************************************************************************************************/
/*                                     WritePixelBuffer                                         */
/************************************************************************************************/
//WritePixelBuffer::WritePixelBuffer()
//{}

// ------------------------------------------------------------------------------------------------
//WritePixelBuffer::WritePixelBuffer( WritePixelBuffer &&other )
//{
//	std::swap( _bufferObject, other._bufferObject );
//}

// ------------------------------------------------------------------------------------------------
//WritePixelBuffer &WritePixelBuffer::operator=( WritePixelBuffer &&other )
//{
//	std::swap( _bufferObject, other._bufferObject );
//	return *this;
//}

// ------------------------------------------------------------------------------------------------
WritePixelBuffer::WritePixelBuffer( int sizeInBytes, const UsageHint &usageHint )
{
	BufferObject::UsageHint usage = BufferObject::UsageHint::StaticDraw;

	switch( usageHint )
	{
	case UsageHint::Stream:  usage = BufferObject::UsageHint::StreamDraw; break;
	case UsageHint::Static:  usage = BufferObject::UsageHint::StaticDraw; break;
	case UsageHint::Dynamic: usage = BufferObject::UsageHint::DynamicDraw; break;
	}

	_bufferObject = BufferObject::New( sizeInBytes, BufferObject::Type::PixelUnpackBuffer, usage );
}

// ------------------------------------------------------------------------------------------------
WritePixelBuffer::~WritePixelBuffer()
{}

// ------------------------------------------------------------------------------------------------
int  WritePixelBuffer::sizeInBytes() const { return _bufferObject->size(); }

// ------------------------------------------------------------------------------------------------
void WritePixelBuffer::bind()       const { _bufferObject->bind(); }
void WritePixelBuffer::unbind()     const { _bufferObject->unbind(); }
void* WritePixelBuffer::mapData()   const { return _bufferObject->mapData( BufferObject::MapMode::WriteOnly );}
bool  WritePixelBuffer::unmapData() const { return _bufferObject->unmapData(); }

// ------------------------------------------------------------------------------------------------
void  WritePixelBuffer::sendData( void *data, int length, int offset ) { _bufferObject->sendData( data, length, offset ); }
void* WritePixelBuffer::receiveData( int length, int offset )          { return _bufferObject->receiveData( length, offset ); }

/************************************************************************************************/
/*                                       ReadPixelBuffer                                        */
/************************************************************************************************/
//ReadPixelBuffer::ReadPixelBuffer()
//{}

// ------------------------------------------------------------------------------------------------
//ReadPixelBuffer::ReadPixelBuffer( ReadPixelBuffer &&other )
//{
//	std::swap( _bufferObject, other._bufferObject );
//}

// ------------------------------------------------------------------------------------------------
//ReadPixelBuffer &ReadPixelBuffer::operator=( ReadPixelBuffer &&other )
//{
//	std::swap( _bufferObject, other._bufferObject );
//	return *this;
//}

// ------------------------------------------------------------------------------------------------
ReadPixelBuffer::ReadPixelBuffer( int sizeInBytes, const UsageHint &usageHint )
{
	BufferObject::UsageHint usage = BufferObject::UsageHint::StaticDraw;

	switch( usageHint )
	{
	case UsageHint::Stream:  usage = BufferObject::UsageHint::StreamRead; break;
	case UsageHint::Static:  usage = BufferObject::UsageHint::StaticRead; break;
	case UsageHint::Dynamic: usage = BufferObject::UsageHint::DynamicRead; break;
	}

	_bufferObject = BufferObject::New( sizeInBytes, BufferObject::Type::PixelPackBuffer, usage );
}

// ------------------------------------------------------------------------------------------------
ReadPixelBuffer::~ReadPixelBuffer()
{}

// ------------------------------------------------------------------------------------------------
int  ReadPixelBuffer::sizeInBytes() const { return _bufferObject->size(); }

// ------------------------------------------------------------------------------------------------
void ReadPixelBuffer::bind()   const { _bufferObject->bind(); }
void ReadPixelBuffer::unbind() const { _bufferObject->unbind(); }
void* ReadPixelBuffer::mapData()   const { return _bufferObject->mapData( BufferObject::MapMode::ReadOnly );}
bool  ReadPixelBuffer::unmapData() const { return _bufferObject->unmapData(); }

// ------------------------------------------------------------------------------------------------
void  ReadPixelBuffer::sendData( void *data, int length, int offset ) { _bufferObject->sendData( data, length, offset ); }
void* ReadPixelBuffer::receiveData( int length, int offset )          { return _bufferObject->receiveData( length, offset ); }