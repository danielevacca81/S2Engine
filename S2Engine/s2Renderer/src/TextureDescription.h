// TextureDescription.h
//
#ifndef TEXTUREDESCRIPTION_ONCE
#define TEXTUREDESCRIPTION_ONCE

#include "s2Renderer_API.h"

namespace s2 {
namespace Renderer {

/************************************************************************************************/
/*                                         TextureFormat                                        */
/************************************************************************************************/
enum class S2RENDERER_API TextureFormat
{
	RedGreenBlue8,
	RedGreenBlue16,
	RedGreenBlueAlpha8,
	RedGreenBlue10A2,
	RedGreenBlueAlpha16,
	Depth16,
	Depth24,
	Red8,
	Red16,
	RedGreen8,
	RedGreen16,
	Red16f,
	Red32f,
	RedGreen16f,
	RedGreen32f,
	Red8i,
	Red8ui,
	Red16i,
	Red16ui,
	Red32i,
	Red32ui,
	RedGreen8i,
	RedGreen8ui,
	RedGreen16i,
	RedGreen16ui,
	RedGreen32i,
	RedGreen32ui,
	RedGreenBlueAlpha32f,
	RedGreenBlue32f,
	RedGreenBlueAlpha16f,
	RedGreenBlue16f,
	Depth24Stencil8,
	Red11fGreen11fBlue10f,
	RedGreenBlue9E5,
	SRedGreenBlue8,
	SRedGreenBlue8Alpha8,
	Depth32f,
	Depth32fStencil8,
	RedGreenBlueAlpha32ui,
	RedGreenBlue32ui,
	RedGreenBlueAlpha16ui,
	RedGreenBlue16ui,
	RedGreenBlueAlpha8ui,
	RedGreenBlue8ui,
	RedGreenBlueAlpha32i,
	RedGreenBlue32i,
	RedGreenBlueAlpha16i,
	RedGreenBlue16i,
	RedGreenBlueAlpha8i,
	RedGreenBlue8i
};


/************************************************************************************************/
/*                                       TextureDescription                                     */
/************************************************************************************************/
class S2RENDERER_API TextureDescription
{
public:
	//TextureDescription();
	TextureDescription( int height, int width, const TextureFormat &format, bool generateMipmaps = false );
	~TextureDescription();

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