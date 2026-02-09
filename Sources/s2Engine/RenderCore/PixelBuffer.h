// PixelBuffer.h
//
#ifndef S2_RENDERCORE_PIXELBUFFER_H
#define S2_RENDERCORE_PIXELBUFFER_H

#include "s2Engine_API.h"

#include "BufferObject.h"

#include <memory>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                     WritePixelBuffer                                         */
/************************************************************************************************/
class S2ENGINE_API WritePixelBuffer
{
public:
	enum class UsageHint
	{
		Stream,
		Static,
		Dynamic
	};

public:
	WritePixelBuffer( int sizeInBytes, const UsageHint &usageHint );
	~WritePixelBuffer();

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
class S2ENGINE_API ReadPixelBuffer
{
public:
	enum class UsageHint
	{
		Stream,
		Static,
		Dynamic
	};

public:
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

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_PIXELBUFFER_H