// PixelBuffer.h
//
#ifndef PIXELBUFFER_ONCE
#define PIXELBUFFER_ONCE

#include "s2OpenGL_API.h"

#include "BufferObject.h"

#include <memory>

namespace s2 {
namespace OpenGL {

/************************************************************************************************/
/*                                     WritePixelBuffer                                         */
/************************************************************************************************/
class S2OPENGL_API WritePixelBuffer
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

	WritePixelBuffer( int sizeInBytes, const UsageHint &usageHint);
	//void set( int sizeInBytes, const UsageHint &usageHint);

	int  sizeInBytes() const;

	void bind() const;
	void unbind() const;

	void  sendData(void *data, int length, int offset = 0);
	void* receiveData(int length, int offset = 0);

private:
	BufferObjectPtr _bufferObject;
};

/************************************************************************************************/
/*                                       ReadPixelBuffer                                        */
/************************************************************************************************/
class S2OPENGL_API ReadPixelBuffer
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
	
	//void ;

	int  sizeInBytes() const;

	void bind() const;
	void unbind() const;

	void  sendData(void *data, int length, int offset = 0);
	void* receiveData(int length, int offset = 0);

private:
	BufferObjectPtr _bufferObject;
};

}
}


#endif