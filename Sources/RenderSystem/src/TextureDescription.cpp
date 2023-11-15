// TextureDescription.cpp
//
#include "TextureDescription.h"

using namespace RenderSystem;

/************************************************************************************************/
/*                                      TextureDescription                                      */
/************************************************************************************************/
//TextureDescription::TextureDescription()
//: TextureDescription( 0,0, TextureFormat::RedGreenBlue8, false )
//{}

// ------------------------------------------------------------------------------------------------
TextureDescription::TextureDescription( int width, int height, const TextureFormat &format, bool generateMipmaps )
: _height( height )
, _width( width )
, _format( format )
, _generateMipmaps( generateMipmaps )
, _rectangle( false )
{
	// compute _rectangle flag based on w and h ?
}

// ------------------------------------------------------------------------------------------------
int           TextureDescription::width()                    const { return _width; }
int           TextureDescription::height()                   const { return _height; }
TextureFormat TextureDescription::textureFormat()            const { return _format; }
bool          TextureDescription::isGenerateMipmapsEnabled() const { return _generateMipmaps; }
bool          TextureDescription::isRectagle()               const { return false; } //@TBD: if true bind to GL_TEXTURE_RECTANGLE instead of GL_TEXTURE_2D

// ------------------------------------------------------------------------------------------------
bool TextureDescription::isColorRenderable() const
{
	return !isDepthRenderable() && !isDepthStencilRenderable();
}

// ------------------------------------------------------------------------------------------------
bool TextureDescription::isDepthRenderable() const
{
	return
		_format == TextureFormat::Depth16 ||
		_format == TextureFormat::Depth24 ||
		_format == TextureFormat::Depth32f ||
		_format == TextureFormat::Depth24Stencil8 ||
		_format == TextureFormat::Depth32fStencil8;
}


// ------------------------------------------------------------------------------------------------
bool TextureDescription::isDepthStencilRenderable() const
{
	return
		_format == TextureFormat::Depth24Stencil8 ||
		_format == TextureFormat::Depth32fStencil8;
}