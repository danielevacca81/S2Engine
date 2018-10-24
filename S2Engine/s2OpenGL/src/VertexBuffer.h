// VertexBuffer.h
// 
#ifndef VERTEXBUFFER_ONCE
#define VERTEXBUFFER_ONCE

#include "s2OpenGL_API.h"

#include "BufferObject.h"

namespace s2 {
namespace OpenGL {

/************************************************************************************************/
/*                                         VertexBuffer                                         */
/************************************************************************************************/
class S2OPENGL_API VertexBuffer
{
public:
	OBJECT_DECLARE_MOVEABLE( VertexBuffer )
	OBJECT_DISABLE_COPY( VertexBuffer )

	VertexBuffer();
	//VertexBuffer( int sizeInBytes, const BufferObject::UsageHint &usageHint );
	~VertexBuffer();

	void set( int sizeInBytes, const BufferObject::UsageHint &usageHint );

	bool isValid()     const { return _bufferObject.id() !=  0; }
	int  sizeInBytes() const { return _bufferObject.size(); }

	void bind() const;
	void unbind() const;

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( const BufferObject::MapMode &mode );
	bool  unmapData();

private:
	BufferObject _bufferObject;
};




} // namespace OpenGL
} // namespace s2

#endif