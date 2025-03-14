// BufferObject.h
// 
#ifndef BUFFEROBJECT_H
#define BUFFEROBJECT_H

#include "RenderCore_API.h"

#include "OpenGLObject.h"

#include <memory>

namespace RenderCore {

/************************************************************************************************/
/*                                      BufferObject                                            */
/************************************************************************************************/
class BufferObject;
typedef std::shared_ptr<BufferObject>   BufferObjectPtr;

class RENDERCORE_API BufferObject: public OpenGLObject
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
	struct Data 
	{
	public:
		Data(): data( nullptr ), size( 0 ) 
		{}
		
		Data( void* data_, int64_t size_ )
		: data( data_ )
		, size( size_ ) 
		{}

		void*   data;
		int64_t size;
	};

public:
	//OBJECT_DISABLE_COPY( BufferObject )
	//OBJECT_DECLARE_MOVEABLE( BufferObject )

	static BufferObjectPtr New( int size, const Type &type, const UsageHint &usageHint );


	//BufferObject();
	BufferObject( int size, const Type &type, const UsageHint &usageHint );
	~BufferObject();

	void create()  override;
	void destroy() override;
	void bind()    const override;
	void unbind()  const override;

	int size()            const { return _size; }
	UsageHint usageHint() const { return _usageHint; }
	Type      type()      const { return _type; }

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );
	void* mapData( const MapMode &mode );
	bool  unmapData();

private:
	void reset() override;

private:
	unsigned int _size; // byte size
	UsageHint    _usageHint;
	Type         _type;
};

} // namespace RenderCore
#endif