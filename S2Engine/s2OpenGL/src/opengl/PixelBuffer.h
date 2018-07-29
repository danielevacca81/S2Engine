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

	/*todo specialized usage hints: *Draw */
	WritePixelBuffer( int sizeInBytes, const BufferObject::BufferUsageHint &usageHint);

	int  sizeInBytes() const;

	void bind();
	void unbind();

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
	/*todo specialized usage hints: *Read */

	ReadPixelBuffer(int sizeInBytes, const BufferObject::BufferUsageHint &usageHint);

	int  sizeInBytes() const;

	void bind();
	void unbind();

	void  sendData(void *data, int length, int offset = 0);
	void* receiveData(int length, int offset = 0);

private:
	BufferObjectPtr _bufferObject;
};

}
}


#endif