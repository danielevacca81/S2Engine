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
	enum BufferUsageHint
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

	enum BufferType
	{
		ArrayBuffer,
		ElementBuffer,
	};

	enum BufferMapMode
	{
		ReadOnly,
		WriteOnly,
		ReadWrite,
	};

public:
	static BufferObjectPtr New( int size, const BufferType &type, const BufferUsageHint &usageHint ) { return std::make_shared<BufferObject>( size, type, usageHint ); }

	BufferObject( int size, const BufferType &type, const BufferUsageHint &usageHint );
	~BufferObject();

	void bind();
	void unbind();

	int size()                  const { return _size; }
	BufferUsageHint usageHint() const { return _usageHint; }
	BufferType      type()      const { return _type; }

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( const BufferMapMode &mode );
	bool  unmapData();

private:
	unsigned int    _id;
	unsigned int    _size; // byte size
	BufferUsageHint _usageHint;
	BufferType      _type;
};

} // namespace OpenGL

}
#endif