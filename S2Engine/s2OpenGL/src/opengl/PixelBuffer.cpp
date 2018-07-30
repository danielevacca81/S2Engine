// PixelBuffer.cpp
//
#include "PixelBuffer.h"

using namespace s2::OpenGL;


/************************************************************************************************/
/*                                     WritePixelBuffer                                         */
/************************************************************************************************/
WritePixelBuffer::WritePixelBuffer(int sizeInBytes, const BufferObject::BufferUsageHint &usageHint)
{
	//assert(); 
	// usage hint must be
	// BufferHint.StreamDraw,
	// BufferHint.StaticDraw,
	// BufferHint.DynamicDraw

	_bufferObject = BufferObject::New( sizeInBytes, BufferObject::PixelUnpackBuffer, usageHint );
}

// ------------------------------------------------------------------------------------------------
int  WritePixelBuffer::sizeInBytes() const { return _bufferObject->size(); }

// ------------------------------------------------------------------------------------------------
void WritePixelBuffer::bind()   { _bufferObject->bind(); }
void WritePixelBuffer::unbind() { _bufferObject->unbind(); }

// ------------------------------------------------------------------------------------------------
void  WritePixelBuffer::sendData(void *data, int length, int offset) { _bufferObject->sendData( data, length, offset ); }
void* WritePixelBuffer::receiveData(int length, int offset )         { return _bufferObject->receiveData( length, offset); }

/************************************************************************************************/
/*                                       ReadPixelBuffer                                        */
/************************************************************************************************/
ReadPixelBuffer::ReadPixelBuffer(int sizeInBytes, const BufferObject::BufferUsageHint &usageHint)
{
	_bufferObject = BufferObject::New(sizeInBytes, BufferObject::PixelPackBuffer, usageHint);
}

// ------------------------------------------------------------------------------------------------
int  ReadPixelBuffer::sizeInBytes() const { return _bufferObject->size(); }

// ------------------------------------------------------------------------------------------------
void ReadPixelBuffer::bind()   { _bufferObject->bind(); }
void ReadPixelBuffer::unbind() { _bufferObject->unbind(); }

// ------------------------------------------------------------------------------------------------
void  ReadPixelBuffer::sendData(void *data, int length, int offset ) { _bufferObject->sendData( data, length, offset ); }
void* ReadPixelBuffer::receiveData(int length, int offset )          { return _bufferObject->receiveData(length,offset); }