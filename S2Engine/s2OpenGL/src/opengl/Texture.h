// Texture.h
//
#ifndef TEXTURE_ONCE
#define TEXTURE_ONCE

#include "s2OpenGL_API.h"

#include "ImageFormat.h"
#include "PixelBuffer.h"
#include "TextureDescription.h"

#include <memory>

namespace s2 {

namespace OpenGL {

/************************************************************************************************/
/*                                          Texture2D                                           */
/************************************************************************************************/
class Texture2D;
typedef std::shared_ptr<Texture2D> Texture2DPtr;

class S2OPENGL_API Texture2D
{
public:	
	static Texture2DPtr New(const TextureDescription &desc);

public:
	Texture2D();
	Texture2D(const TextureDescription &, void *data = nullptr);
	virtual ~Texture2D();

	TextureDescription description() const;
	unsigned int id() const;

	void bind();
	static void unbind();

	void update( int xOffset, int yOffset, 
				 int width, int height, 
				 const ImageFormat &imgFormat,
				 const ImageDataType &imgDataType,
				 void* pixels/*, int rowAlignment = 4 */);

	void update( int xOffset, int yOffset, 
				 int width, int height, 
				 const ImageFormat &imgFormat,
				 const ImageDataType &imgDataType,
				 const ReadPixelBuffer &gpuBuffer/*, int rowAlignment = 4 */);
	
	
	//void clear();

	/* todo: fromBufferWritePixelBuffer pixelBuffer,
	int xOffset,
		int yOffset,
		int width,
		int height,
		ImageFormat format,
		ImageDatatype dataType,
		int rowAlignment)
*/
private:
	void validateAlignment( int );
	void setDefaultSampler();
	void generateMipmaps();


private:
	unsigned int _id;
	TextureFormat        _format;
	TextureDescription _description;
};

}
}


#endif