// Texture.h
//
#ifndef TEXTURE_ONCE
#define TEXTURE_ONCE

#include "s2OpenGL_API.h"


#include "TextureFormat.h"
#include "TextureDescription.h"

#include <memory>

namespace s2 {

namespace OpenGL {

// ------------------------------------------------------------------------------------------------

//typedef std::shared_ptr<Texture>   TexturePtr;
//typedef std::shared_ptr<Texture1D> Texture1DPtr;


/************************************************************************************************/
/*                                          Texture1D                                           */
/************************************************************************************************/
//class S2OPENGL_API Texture1D : public Texture
//{
//public:	
//	static Texture1DPtr New() { return std::make_shared<Texture1D>(); }
//	static Texture1DPtr New(int channelCount, const DataFormat &format, const DataType &type,bool mipmaps,int width,void *pixels)
//	{ Texture1DPtr t = std::make_shared<Texture1D>(); t->create(channelCount,format,type,mipmaps,width,pixels); return t;}
//
//public:
//	Texture1D();
//
//	TextureType	type()	const;
//
//	void create(
//		int channelCount,
//		const DataFormat &format,
//		const DataType &type,
//		bool mipmaps,
//		int   width,
//		void *pixels);
//
//	void clear();
//
//	virtual int     width()     const { return _textureWidth; }
//	virtual int     height()    const { return 1; }
//
//protected:
//	int _textureWidth;
//};


/************************************************************************************************/
/*                                          Texture2D                                           */
/************************************************************************************************/
class Texture2D;
typedef std::shared_ptr<Texture2D> Texture2DPtr;

class S2OPENGL_API Texture2D
{
public:	
	static Texture2DPtr New(const Texture2DDescription &desc);

public:
	Texture2D();
	Texture2D(const Texture2DDescription &, void *data = nullptr);
	virtual ~Texture2D();

	Texture2DDescription description() const;
	unsigned int id() const;

	void bind();
	static void unbind();

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
	void setDefaultSampler();
	void generateMipmaps();


private:
	unsigned int _id;
	TextureFormat        _format;
	Texture2DDescription _description;
};

}
}


#endif