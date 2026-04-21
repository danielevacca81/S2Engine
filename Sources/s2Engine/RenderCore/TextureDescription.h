// TextureDescription.h
//
#ifndef S2_RENDERCORE_TEXTUREDESCRIPTION_H
#define S2_RENDERCORE_TEXTUREDESCRIPTION_H

#include "s2Engine_API.h"

#include <string>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                         TextureFormat                                        */
/************************************************************************************************/
enum class S2ENGINE_API TextureFormat
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
class S2ENGINE_API TextureDescription
{
public:
    // Constructor with essential parameters only
    TextureDescription( 
        int width, 
        int height, 
        TextureFormat format, 
        bool generateMipmaps = false,
        const std::string& name = "" );

    // Default constructor
    TextureDescription() 
        : TextureDescription( 1, 1, TextureFormat::RedGreenBlueAlpha8, false, "" ) 
    {}

    // Getters (                    const methods)
    int width()                     const { return _width; }
    int height()                    const { return _height; }
    TextureFormat textureFormat()   const { return _format; }
    bool isGenerateMipmapsEnabled() const { return _generateMipmaps; }
    std::string name()              const { return _name; }

    // Format queries
    bool isColorRenderable() const;
    bool isDepthRenderable() const;
    bool isDepthStencilRenderable() const;
    
    // Format properties
    int  channelCount()       const;
    bool isIntegerFormat()    const;
    bool isFloatFormat()      const;
    bool isNormalizedFormat() const;
    bool isSRGBFormat()       const;

private:
    int           _width;
    int           _height;
    TextureFormat _format;
    bool          _generateMipmaps;
    std::string   _name; // For debug labels
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_TEXTUREDESCRIPTION_H