// IndexBuffer.h
// 
#ifndef INDEXBUFFER_ONCE
#define INDEXBUFFER_ONCE

#include "s2OpenGL_API.h"

#include "BufferObject.h"

namespace s2 {

namespace OpenGL {

/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
class S2OPENGL_API IndexBuffer
{
public:
	enum IndexDataType
	{
		UnsignedInt,
		UnsignedShort,
	};

public:
	IndexBuffer();
	IndexBuffer( int sizeInBytes, const IndexDataType &dataType, const BufferObject::BufferUsageHint &usageHint );
	~IndexBuffer();

	void set( int sizeInBytes, const IndexDataType &dataType, const BufferObject::BufferUsageHint &usageHint );

	bool isValid() const { return _valid; }

	void bind();
	void unbind();

	int count() const { return _count; }

	IndexDataType dataType() const { return _dataType; }

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( BufferObject::BufferMapMode mode );
	bool  unmapData();

private:
	BufferObjectPtr _bufferObject;
	IndexDataType   _dataType;
	int             _count;
	bool            _valid;

};

} // namespace OpenGL

}
#endif