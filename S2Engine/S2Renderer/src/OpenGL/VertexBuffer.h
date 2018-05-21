// VertexBuffer.h
// 
#ifndef VERTEXBUFFER_ONCE
#define VERTEXBUFFER_ONCE

#include "S2RendererAPI.h"

#include "BufferObject.h"

namespace OpenGL {

/************************************************************************************************/
/*                                         VertexBuffer                                         */
/************************************************************************************************/
class S2RENDERER_API VertexBuffer
{
private:
	BufferObjectPtr _bufferObject;
	bool            _valid;

public:
	VertexBuffer();
	VertexBuffer( int sizeInBytes, BufferObject::BufferUsageHint usageHint );
	~VertexBuffer();

	void set( int sizeInBytes, BufferObject::BufferUsageHint usageHint );

	bool isValid()     const { return _valid; }
	int  sizeInBytes() const { return _bufferObject->size(); }

	void bind();
	void unbind();

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( BufferObject::BufferMapMode mode );
	bool  unmapData();
};

} // namespace OpenGL

#endif