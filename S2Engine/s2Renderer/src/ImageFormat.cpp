// ImageFormat.cpp
//
#include "ImageFormat.h"

namespace Renderer {

// -------------------------------------------------------------------------------------------------
int numberOfChannels( const ImageFormat &format )
{
	switch( format )
	{
	case ImageFormat::StencilIndex:             return 1;
	case ImageFormat::DepthComponent:           return 1;
	case ImageFormat::Red:                      return 1;
	case ImageFormat::Green:                    return 1;
	case ImageFormat::Blue:                     return 1;
	case ImageFormat::RedGreenBlue:		        return 3;
	case ImageFormat::RedGreenBlueAlpha:        return 4;
	case ImageFormat::BlueGreenRed:		        return 3;
	case ImageFormat::BlueGreenRedAlpha:        return 4;
	case ImageFormat::RedGreen:                 return 2;
	case ImageFormat::RedGreenInteger:          return 2;
	case ImageFormat::DepthStencil:             return 2;
	case ImageFormat::RedInteger:               return 1;
	case ImageFormat::GreenInteger:             return 1;
	case ImageFormat::BlueInteger:              return 1;
	case ImageFormat::RedGreenBlueInteger:      return 3;
	case ImageFormat::RedGreenBlueAlphaInteger: return 4;
	case ImageFormat::BlueGreenRedInteger:      return 3;
	case ImageFormat::BlueGreenRedAlphaInteger: return 4;
	}
	//assert();
	return -1;
}

// -------------------------------------------------------------------------------------------------
int sizeInBytes( const ImageDataType &dataType )
{
	switch( dataType )
	{
	case ImageDataType::Byte:                          return 1;
	case ImageDataType::UnsignedByte:                  return 1;
	case ImageDataType::Short:                         return 2;
	case ImageDataType::UnsignedShort:                 return 2;
	case ImageDataType::Int:                           return 4;
	case ImageDataType::UnsignedInt:                   return 4;
	case ImageDataType::Float:                         return 4;
	case ImageDataType::HalfFloat:                     return 2;
	case ImageDataType::UnsignedByte332:               return 1;
	case ImageDataType::UnsignedShort4444:             return 2;
	case ImageDataType::UnsignedShort5551:             return 2;
	case ImageDataType::UnsignedInt8888:               return 4;
	case ImageDataType::UnsignedInt1010102:            return 4;
	case ImageDataType::UnsignedByte233Reversed:       return 1;
	case ImageDataType::UnsignedShort565:              return 2;
	case ImageDataType::UnsignedShort565Reversed:      return 2;
	case ImageDataType::UnsignedShort4444Reversed:     return 2;
	case ImageDataType::UnsignedShort1555Reversed:     return 2;
	case ImageDataType::UnsignedInt8888Reversed:       return 4;
	case ImageDataType::UnsignedInt2101010Reversed:    return 4;
	case ImageDataType::UnsignedInt248:                return 4;
	case ImageDataType::UnsignedInt10F11F11FReversed:  return 4;
	case ImageDataType::UnsignedInt5999Reversed:       return 4;
	case ImageDataType::Float32UnsignedInt248Reversed: return 4;
	}

	//assert();
	return -1;
}

// -------------------------------------------------------------------------------------------------
int computeRequiredSizeInBytes( int width, int height, const ImageFormat &format, const ImageDataType &dataType, int rowAlignment )
{
	int rowSize = width * numberOfChannels( format ) * sizeInBytes( dataType );

	int remainder = ( rowSize % rowAlignment );
	rowSize += ( rowAlignment - remainder ) % rowAlignment;

	return rowSize * height;
}

}