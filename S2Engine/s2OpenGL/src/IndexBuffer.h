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
	OBJECT_DECLARE_MOVEABLE( IndexBuffer )
	OBJECT_DISABLE_COPY( IndexBuffer )

	IndexBuffer();
	//IndexBuffer( int sizeInBytes, const IndexDataType &dataType, const BufferObject::UsageHint &usageHint );

	void set( int sizeInBytes, const IndexDataType &dataType, const BufferObject::UsageHint &usageHint );

	bool isValid() const { return _valid; }

	void bind();
	void unbind();

	int count() const { return _count; }

	IndexDataType dataType() const { return _dataType; }

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( const BufferObject::MapMode &mode );
	bool  unmapData();

private:
	BufferObject    _bufferObject;
	IndexDataType   _dataType;
	int             _count;
	bool            _valid;

};

} // namespace OpenGL

}
#endif