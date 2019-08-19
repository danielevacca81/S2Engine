// PixelBuffer.h
//
#ifndef PIXELBUFFER_ONCE
#define PIXELBUFFER_ONCE

#include "s2Renderer_API.h"

#include "BufferObject.h"

#include <memory>

namespace s2 {
namespace Renderer {

/************************************************************************************************/
/*                                     WritePixelBuffer                                         */
/************************************************************************************************/
class S2RENDERER_API WritePixelBuffer
{
public:
	enum class UsageHint
	{
		Stream,
		Static,
		Dynamic
	};

public:
	//OBJECT_DECLARE_MOVEABLE( WritePixelBuffer )
	//OBJECT_DISABLE_COPY( WritePixelBuffer )

	WritePixelBuffer( int sizeInBytes, const UsageHint &usageHint );
	~WritePixelBuffer();
	//void set( int sizeInBytes, const UsageHint &usageHint);

	int  sizeInBytes() const;

	void bind() const;
	void unbind() const;
	void* mapData()   const;
	bool  unmapData() const;

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );

private:
	BufferObjectPtr _bufferObject;
};

/************************************************************************************************/
/*                                       ReadPixelBuffer                                        */
/************************************************************************************************/
class S2RENDERER_API ReadPixelBuffer
{
public:
	enum class UsageHint
	{
		Stream,
		Static,
		Dynamic
	};

public:
	//OBJECT_DECLARE_MOVEABLE( ReadPixelBuffer )
	//OBJECT_DISABLE_COPY( ReadPixelBuffer )

	ReadPixelBuffer( int sizeInBytes, const UsageHint &usageHint );
	~ReadPixelBuffer();

	int  sizeInBytes() const;

	void bind() const;
	void unbind() const;
	void* mapData()   const;
	bool  unmapData() const;

	void  sendData( void *data, int length, int offset = 0 );
	void* receiveData( int length, int offset = 0 );

private:
	BufferObjectPtr _bufferObject;
};

}
}


#endif