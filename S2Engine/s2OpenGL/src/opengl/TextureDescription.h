// TextureDescription.h
//
#ifndef TEXTUREDESCRIPTION_ONCE
#define TEXTUREDESCRIPTION_ONCE

#include "s2OpenGL_API.h"

#include "TextureFormat.h"

#include <memory>

namespace s2 {
namespace OpenGL {

/************************************************************************************************/
/*                                      Texture1DDescription                                      */
/************************************************************************************************/
class S2OPENGL_API Texture1DDescription
{

public:
	Texture1DDescription( int width );
	~Texture1DDescription();
};


/************************************************************************************************/
/*                                      Texture2DDescription                                      */
/************************************************************************************************/
class S2OPENGL_API Texture2DDescription
{

public:
	Texture2DDescription();
	Texture2DDescription( int height, int width, const TextureFormat &format, bool generateMipmaps = false );
	~Texture2DDescription();

	int width() const;
	int height() const;
	TextureFormat textureFormat() const;
	
	bool isRectagle() const;
	bool isGenerateMipmapsEnabled() const;
	bool isColorRenderable() const;
	bool isDepthRenderable() const;
	bool isDepthStencilRenderable() const;

private:
	int           _height;
	int           _width;
	TextureFormat _format;
	bool          _generateMipmaps;
	bool          _rectangle;
};




}
}


#endif