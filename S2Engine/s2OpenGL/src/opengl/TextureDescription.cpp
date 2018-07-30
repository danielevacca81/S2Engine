// TextureDescription.cpp
//
#include "TextureDescription.h"

using namespace s2::OpenGL;

/************************************************************************************************/
/*                                      Texture1DDescription                                      */
/************************************************************************************************/
Texture1DDescription::Texture1DDescription(int width)
{}

Texture1DDescription::~Texture1DDescription()
{}

/************************************************************************************************/
/*                                      Texture2DDescription                                      */
/************************************************************************************************/
Texture2DDescription::Texture2DDescription()
: _height(0)
, _width(0)
, _format( TextureFormat::RedGreenBlue8 )
, _generateMipmaps(false)
, _rectangle(false)
{}

// ------------------------------------------------------------------------------------------------
Texture2DDescription::Texture2DDescription( int width, int height, const TextureFormat &format, bool generateMipmaps)
: _height(height)
, _width(width)
, _format(format)
, _generateMipmaps(generateMipmaps)
, _rectangle(false)
{
	// compute _rectangle flag based on w and h ?
}

// ------------------------------------------------------------------------------------------------
Texture2DDescription::~Texture2DDescription()
{}
	
// ------------------------------------------------------------------------------------------------
int           Texture2DDescription::width() const { return _width; }
int           Texture2DDescription::height() const { return _height; }
TextureFormat Texture2DDescription::textureFormat() const { return _format; }
bool          Texture2DDescription::isGenerateMipmapsEnabled() const { return _generateMipmaps; }
bool          Texture2DDescription::isRectagle() const { return false; } //@TBD: if true bind to GL_TEXTURE_RECTANGLE instead of GL_TEXTURE_2D

// ------------------------------------------------------------------------------------------------
bool Texture2DDescription::isColorRenderable() const
{
	return !isDepthRenderable() && !isDepthStencilRenderable();
}

// ------------------------------------------------------------------------------------------------
bool Texture2DDescription::isDepthRenderable() const
{
	return
		_format == TextureFormat::Depth16 ||
		_format == TextureFormat::Depth24 ||
		_format == TextureFormat::Depth32f ||
		_format == TextureFormat::Depth24Stencil8 ||
		_format == TextureFormat::Depth32fStencil8;
}


// ------------------------------------------------------------------------------------------------
bool Texture2DDescription::isDepthStencilRenderable() const
{
	return
		_format == TextureFormat::Depth24Stencil8 ||
		_format == TextureFormat::Depth32fStencil8;
}