// IndexBuffer.h
// 
#ifndef S2_RENDERCORE_INDEXBUFFER_H
#define S2_RENDERCORE_INDEXBUFFER_H

#include "s2Engine_API.h"

#include "BufferObject.h"

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
class S2ENGINE_API IndexBuffer
{
public:
	enum IndexDataType
	{
		UnsignedInt,
		UnsignedShort,
	};

public:
	//OBJECT_DECLARE_MOVEABLE( IndexBuffer )
	//OBJECT_DISABLE_COPY( IndexBuffer )

	IndexBuffer();
	IndexBuffer( int sizeInBytes, const IndexDataType &dataType, const BufferObject::UsageHint &usageHint );

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
	BufferObjectPtr _bufferObject;
	IndexDataType   _dataType;
	int             _count;
	bool            _valid;

};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_INDEXBUFFER_H