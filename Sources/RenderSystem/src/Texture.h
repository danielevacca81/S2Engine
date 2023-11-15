// Texture.h
//
#ifndef TEXTURE_ONCE
#define TEXTURE_ONCE

#include "RenderSystem_API.h"

#include "ImageFormat.h"
#include "PixelBuffer.h"
#include "TextureDescription.h"

#include "OpenGLObject.h"

#include "Core/Pixmap.h"

#include <memory>

namespace RenderSystem {

/************************************************************************************************/
/*                                          Texture2D                                           */
/************************************************************************************************/
class Texture2D;
typedef std::shared_ptr<Texture2D> Texture2DPtr;

class RENDERSYSTEM_API Texture2D : public OpenGLObject
{
public:	
	static Texture2DPtr New( const TextureDescription &desc, void *data = nullptr );

public:
	//OBJECT_DECLARE_MOVEABLE( Texture2D )
	//OBJECT_DISABLE_COPY( Texture2D )

	Texture2D( const TextureDescription &description, void *data );
	~Texture2D();

	TextureDescription description() const;

	void bind()    const override;
	void unbind()  const override;
	void create()  override;
	void destroy() override;

	static void unbindAll();

	void setData( void* pixels /*, int rowAlignment = 4 */ );

	void resize( const int width, const int height );

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
	
	Pixmap<uint8_t> readData() const;

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
    int  objectLabelIdentifier() const override;

private:
	TextureDescription _description;
};

}
#endif