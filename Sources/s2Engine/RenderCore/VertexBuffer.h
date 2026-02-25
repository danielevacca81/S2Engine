// VertexBuffer.h
// 
#ifndef S2_RENDERCORE_VERTEXBUFFER_H
#define S2_RENDERCORE_VERTEXBUFFER_H

#include "s2Engine_API.h"

#include "GPUBufferObject.h"

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                         VertexBuffer                                         */
/************************************************************************************************/
class S2ENGINE_API VertexBuffer
{
public:
	VertexBuffer() = default;
	VertexBuffer( int sizeInBytes, const GPUBufferObject::UsageHint &usageHint );
	VertexBuffer( void *data, int sizeInBytes, const GPUBufferObject::UsageHint &usageHint );

	void set( int sizeInBytes, const GPUBufferObject::UsageHint &usageHint );

	bool isValid()     const { return _bufferObject->id() !=  0; }
	int  sizeInBytes() const { return _bufferObject->size(); }

	void bind() const;
	void unbind() const;

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( const GPUBufferObject::MapMode &mode );
	bool  unmapData();

private:
	GPUBufferObjectPtr _bufferObject;
};




} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_VERTEXBUFFER_H