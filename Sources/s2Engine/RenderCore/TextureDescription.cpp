// TextureDescription.cpp
//
#include "TextureDescription.h"

using namespace s2::RenderCore;

/************************************************************************************************/
/*                                      TextureDescription                                      */
/************************************************************************************************/

// ------------------------------------------------------------------------------------------------
TextureDescription::TextureDescription( 
    int width, 
    int height, 
    TextureFormat format, 
    bool generateMipmaps,
    const std::string& name )
    : _width( width )
    , _height( height )
    , _format( format )
    , _generateMipmaps( generateMipmaps )
    , _name( name )
{
}

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

// ------------------------------------------------------------------------------------------------
int TextureDescription::channelCount() const
{
    switch( _format )
    {
    // 1 channel
    case TextureFormat::Red8:
    case TextureFormat::Red16:
    case TextureFormat::Red16f:
    case TextureFormat::Red32f:
    case TextureFormat::Red8i:
    case TextureFormat::Red8ui:
    case TextureFormat::Red16i:
    case TextureFormat::Red16ui:
    case TextureFormat::Red32i:
    case TextureFormat::Red32ui:
    case TextureFormat::Depth16:
    case TextureFormat::Depth24:
    case TextureFormat::Depth32f:
        return 1;

    // 2 channels
    case TextureFormat::RedGreen8:
    case TextureFormat::RedGreen16:
    case TextureFormat::RedGreen16f:
    case TextureFormat::RedGreen32f:
    case TextureFormat::RedGreen8i:
    case TextureFormat::RedGreen8ui:
    case TextureFormat::RedGreen16i:
    case TextureFormat::RedGreen16ui:
    case TextureFormat::RedGreen32i:
    case TextureFormat::RedGreen32ui:
    case TextureFormat::Depth24Stencil8:
    case TextureFormat::Depth32fStencil8:
        return 2;

    // 3 channels
    case TextureFormat::RedGreenBlue8:
    case TextureFormat::RedGreenBlue16:
    case TextureFormat::RedGreenBlue32f:
    case TextureFormat::RedGreenBlue16f:
    case TextureFormat::Red11fGreen11fBlue10f:
    case TextureFormat::RedGreenBlue9E5:
    case TextureFormat::SRedGreenBlue8:
    case TextureFormat::RedGreenBlue32ui:
    case TextureFormat::RedGreenBlue16ui:
    case TextureFormat::RedGreenBlue8ui:
    case TextureFormat::RedGreenBlue32i:
    case TextureFormat::RedGreenBlue16i:
    case TextureFormat::RedGreenBlue8i:
        return 3;

    // 4 channels
    case TextureFormat::RedGreenBlueAlpha8:
    case TextureFormat::RedGreenBlue10A2:
    case TextureFormat::RedGreenBlueAlpha16:
    case TextureFormat::RedGreenBlueAlpha32f:
    case TextureFormat::RedGreenBlueAlpha16f:
    case TextureFormat::SRedGreenBlue8Alpha8:
    case TextureFormat::RedGreenBlueAlpha32ui:
    case TextureFormat::RedGreenBlueAlpha16ui:
    case TextureFormat::RedGreenBlueAlpha8ui:
    case TextureFormat::RedGreenBlueAlpha32i:
    case TextureFormat::RedGreenBlueAlpha16i:
    case TextureFormat::RedGreenBlueAlpha8i:
        return 4;

    default:
        return 0;
    }
}

// ------------------------------------------------------------------------------------------------
bool TextureDescription::isIntegerFormat() const
{
    return
        _format == TextureFormat::Red8i ||
        _format == TextureFormat::Red8ui ||
        _format == TextureFormat::Red16i ||
        _format == TextureFormat::Red16ui ||
        _format == TextureFormat::Red32i ||
        _format == TextureFormat::Red32ui ||
        _format == TextureFormat::RedGreen8i ||
        _format == TextureFormat::RedGreen8ui ||
        _format == TextureFormat::RedGreen16i ||
        _format == TextureFormat::RedGreen16ui ||
        _format == TextureFormat::RedGreen32i ||
        _format == TextureFormat::RedGreen32ui ||
        _format == TextureFormat::RedGreenBlue8i ||
        _format == TextureFormat::RedGreenBlue16i ||
        _format == TextureFormat::RedGreenBlue32i ||
        _format == TextureFormat::RedGreenBlue8ui ||
        _format == TextureFormat::RedGreenBlue16ui ||
        _format == TextureFormat::RedGreenBlue32ui ||
        _format == TextureFormat::RedGreenBlueAlpha8i ||
        _format == TextureFormat::RedGreenBlueAlpha16i ||
        _format == TextureFormat::RedGreenBlueAlpha32i ||
        _format == TextureFormat::RedGreenBlueAlpha8ui ||
        _format == TextureFormat::RedGreenBlueAlpha16ui ||
        _format == TextureFormat::RedGreenBlueAlpha32ui;
}

// ------------------------------------------------------------------------------------------------
bool TextureDescription::isFloatFormat() const
{
    return
        _format == TextureFormat::Red16f ||
        _format == TextureFormat::Red32f ||
        _format == TextureFormat::RedGreen16f ||
        _format == TextureFormat::RedGreen32f ||
        _format == TextureFormat::RedGreenBlue16f ||
        _format == TextureFormat::RedGreenBlue32f ||
        _format == TextureFormat::RedGreenBlueAlpha16f ||
        _format == TextureFormat::RedGreenBlueAlpha32f ||
        _format == TextureFormat::Red11fGreen11fBlue10f ||
        _format == TextureFormat::RedGreenBlue9E5 ||
        _format == TextureFormat::Depth32f ||
        _format == TextureFormat::Depth32fStencil8;
}

// ------------------------------------------------------------------------------------------------
bool TextureDescription::isNormalizedFormat() const
{
    return
        _format == TextureFormat::Red8 ||
        _format == TextureFormat::Red16 ||
        _format == TextureFormat::RedGreen8 ||
        _format == TextureFormat::RedGreen16 ||
        _format == TextureFormat::RedGreenBlue8 ||
        _format == TextureFormat::RedGreenBlue16 ||
        _format == TextureFormat::RedGreenBlueAlpha8 ||
        _format == TextureFormat::RedGreenBlueAlpha16 ||
        _format == TextureFormat::RedGreenBlue10A2 ||
        _format == TextureFormat::Depth16 ||
        _format == TextureFormat::Depth24 ||
        _format == TextureFormat::Depth24Stencil8;
}

// ------------------------------------------------------------------------------------------------
bool TextureDescription::isSRGBFormat() const
{
    return
        _format == TextureFormat::SRedGreenBlue8 ||
        _format == TextureFormat::SRedGreenBlue8Alpha8;
}