// BufferObject.h
// 
#ifndef BUFFEROBJECT_ONCE
#define BUFFEROBJECT_ONCE

#include "s2OpenGL_API.h"

#include <memory>

namespace s2 {

namespace OpenGL {

class BufferObject;
typedef std::shared_ptr<BufferObject>   BufferObjectPtr;

/************************************************************************************************/
/*                                      BufferObject                                            */
/************************************************************************************************/
class S2OPENGL_API BufferObject
{
public:
	enum class UsageHint
	{
		StreamDraw,
		StreamRead,
		StreamCopy,
		StaticDraw,
		StaticRead,
		StaticCopy,
		DynamicDraw,
		DynamicRead,
		DynamicCopy,
	};

	enum class Type
	{
		ArrayBuffer,
		ElementBuffer,
		PixelUnpackBuffer,
		PixelPackBuffer
	};

	enum class MapMode
	{
		ReadOnly,
		WriteOnly,
		ReadWrite,
	};

public:
	static BufferObjectPtr New( int size, const Type &type, const UsageHint &usageHint ) { return std::make_shared<BufferObject>( size, type, usageHint ); }

	BufferObject( int size, const Type &type, const UsageHint &usageHint );
	~BufferObject();

	void bind();
	void unbind();

	int size()            const { return _size; }
	UsageHint usageHint() const { return _usageHint; }
	Type      type()      const { return _type; }

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( const MapMode &mode );
	bool  unmapData();

private:
	unsigned int _id;
	unsigned int _size; // byte size
	UsageHint    _usageHint;
	Type   _type;
};

} // namespace OpenGL

}
#endif