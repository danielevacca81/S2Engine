// BufferObject.h
// 
#ifndef BUFFEROBJECT_ONCE
#define BUFFEROBJECT_ONCE

#include "S2RendererAPI.h"

#include <memory>

namespace OpenGL {

class BufferObject;
typedef std::shared_ptr<BufferObject>   BufferObjectPtr;

/************************************************************************************************/
/*                                      BufferObject                                            */
/************************************************************************************************/
class S2RENDERER_API BufferObject
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

private:
	unsigned int    _id;
	unsigned int    _size; // byte size
	BufferUsageHint _usageHint;
	BufferType      _type;

public:
	static BufferObjectPtr New( int size, BufferType type, BufferUsageHint usageHint ) { return std::make_shared<BufferObject>(size,type,usageHint); }

	BufferObject( int size, BufferType type, BufferUsageHint usageHint );
	~BufferObject();

	void bind();
	void unbind();

	int size()                  const { return _size; }
	BufferUsageHint usageHint() const { return _usageHint; }
	BufferType      type()      const { return _type;}

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( BufferMapMode mode );
	bool  unmapData();
};

} // namespace OpenGL

#endif