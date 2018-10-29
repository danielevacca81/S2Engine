// Texture.h
//
#ifndef TEXTURE_ONCE
#define TEXTURE_ONCE

#include "s2Renderer_API.h"

#include "ImageFormat.h"
#include "PixelBuffer.h"
#include "TextureDescription.h"

#include "OpenGLObject.h"

#include "graphics/ImageBuffer.h"

#include <memory>

namespace s2 {
namespace Renderer {

/************************************************************************************************/
/*                                          Texture2D                                           */
/************************************************************************************************/
class Texture2D;
typedef std::shared_ptr<Texture2D> Texture2DPtr;

class S2RENDERER_API Texture2D : public OpenGLObject
{
public:	
	static Texture2DPtr makeNew( const TextureDescription &desc, void *data = nullptr );

public:
	//OBJECT_DECLARE_MOVEABLE( Texture2D )
	//OBJECT_DISABLE_COPY( Texture2D )

	Texture2D( const TextureDescription &description, void *data );
	~Texture2D();

	TextureDescription description() const;

	void bind()    const override;
	void unbind()  const override;
	bool create()  override;
	void destroy() override;

	static void unbindAll();

	void setData( void* pixels /*, int rowAlignment = 4 */ );

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
	
	ImageBufferPtr<unsigned char> readData() const;

	/* todo: 
	 void clear();
	
	fromBufferWritePixelBuffer pixelBuffer,
	int xOffset,
		int yOffset,
		int width,
		int height,
		ImageFormat format,
		ImageDatatype dataType,
		int rowAlignment)
*/
private:
	void reset() override;
	void validateAlignment( int );
	void setDefaultSampler();
	void generateMipmaps();


private:
	TextureFormat      _format;
	TextureDescription _description;
};

}
}


#endif