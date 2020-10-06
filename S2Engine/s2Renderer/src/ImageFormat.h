// ImageFormat.h
//
#ifndef IMAGEFORMAT_ONCE
#define IMAGEFORMAT_ONCE

#include "s2Renderer_API.h"

namespace Renderer {

/************************************************************************************************/
/*                                          ImageFormat                                           */
/************************************************************************************************/
enum class S2RENDERER_API ImageFormat
{
    StencilIndex,
    DepthComponent,
    Red,
    Green,
    Blue,
    RedGreenBlue,
    RedGreenBlueAlpha,
    BlueGreenRed,
    BlueGreenRedAlpha,
    RedGreen,
    RedGreenInteger,
    DepthStencil,
    RedInteger,
    GreenInteger,
    BlueInteger,
    RedGreenBlueInteger,
    RedGreenBlueAlphaInteger,
    BlueGreenRedInteger,
    BlueGreenRedAlphaInteger
};

/************************************************************************************************/
/*                                        ImageDataType                                         */
/************************************************************************************************/
enum class S2RENDERER_API ImageDataType
{
	Byte,
	UnsignedByte,
	Short,
	UnsignedShort,
	Int,
	UnsignedInt,
	Float,
	HalfFloat,
	UnsignedByte332,
	UnsignedShort4444,
	UnsignedShort5551,
	UnsignedInt8888,
	UnsignedInt1010102,
	UnsignedByte233Reversed,
	UnsignedShort565,
	UnsignedShort565Reversed,
	UnsignedShort4444Reversed,
	UnsignedShort1555Reversed,
	UnsignedInt8888Reversed,
	UnsignedInt2101010Reversed,
	UnsignedInt248,
	UnsignedInt10F11F11FReversed,
	UnsignedInt5999Reversed,
	Float32UnsignedInt248Reversed
};

int numberOfChannels( const ImageFormat &format );
int sizeInBytes( const ImageDataType &dataType );
int computeRequiredSizeInBytes( int width, int height, const ImageFormat &format, const ImageDataType &dataType, int rowAlignment );

}
#endif