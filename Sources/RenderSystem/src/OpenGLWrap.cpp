// OpenGLWrap.cpp
// 
#include "OpenGLWrap.h"

#include "OpenGL.h"

namespace RenderSystem {

/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
#pragma region IndexBuffer
// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const IndexBuffer::IndexDataType &datatype )
{
	switch( datatype )
	{
	case IndexBuffer::UnsignedInt:    return GL_UNSIGNED_INT;
	case IndexBuffer::UnsignedShort:  return GL_UNSIGNED_SHORT;
	}
	assert( ( "Unknown IndexBuffer::IndexDataType", 0 ) );
	return -1;
}
#pragma endregion


/************************************************************************************************/
/*                                         AttributeBuffer                                      */
/************************************************************************************************/
#pragma region AttributeBuffer
// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const AttributeBuffer::ComponentDatatype &datatype )
{
	switch( datatype )
	{
	case AttributeBuffer::Byte:          return GL_BYTE;
	case AttributeBuffer::UnsignedByte:  return GL_UNSIGNED_BYTE;
	case AttributeBuffer::Short:         return GL_SHORT;
	case AttributeBuffer::UnsignedShort: return GL_UNSIGNED_SHORT;
	case AttributeBuffer::Int:           return GL_INT;
	case AttributeBuffer::UnsignedInt:   return GL_UNSIGNED_INT;
	case AttributeBuffer::Float:         return GL_FLOAT;
	case AttributeBuffer::HalfFloat:     return GL_HALF_FLOAT;
	case AttributeBuffer::Double:        return GL_DOUBLE;
	}

	assert( ( "Unknown AttributeBuffer::ComponentDatatype", 0 ) );
	return -1;
}
#pragma endregion

/************************************************************************************************/
/*                                           BufferObject                                       */
/************************************************************************************************/
#pragma region BufferObject
// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const BufferObject::UsageHint &hint )
{
	switch( hint )
	{
	case BufferObject::UsageHint::StreamDraw:  return GL_STREAM_DRAW;
	case BufferObject::UsageHint::StreamRead:  return GL_STREAM_READ;
	case BufferObject::UsageHint::StreamCopy:  return GL_STREAM_COPY;
	case BufferObject::UsageHint::StaticDraw:  return GL_STATIC_DRAW;
	case BufferObject::UsageHint::StaticRead:  return GL_STATIC_READ;
	case BufferObject::UsageHint::StaticCopy:  return GL_STATIC_COPY;
	case BufferObject::UsageHint::DynamicDraw: return GL_DYNAMIC_DRAW;
	case BufferObject::UsageHint::DynamicRead: return GL_DYNAMIC_READ;
	case BufferObject::UsageHint::DynamicCopy: return GL_DYNAMIC_COPY;
	}

	assert( ( "Unknown BufferObject::UsageHint", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const BufferObject::Type &type )
{
	switch( type )
	{
	case BufferObject::Type::ArrayBuffer:       return GL_ARRAY_BUFFER;
	case BufferObject::Type::ElementBuffer:     return GL_ELEMENT_ARRAY_BUFFER;
	case BufferObject::Type::PixelPackBuffer:   return GL_PIXEL_PACK_BUFFER;
	case BufferObject::Type::PixelUnpackBuffer: return GL_PIXEL_UNPACK_BUFFER;
	}

	assert( ( "Unknown BufferObject::Type", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const BufferObject::MapMode &mode )
{
	switch( mode )
	{
	case BufferObject::MapMode::ReadOnly:  return GL_READ_ONLY;
	case BufferObject::MapMode::WriteOnly: return GL_WRITE_ONLY;
	case BufferObject::MapMode::ReadWrite: return GL_READ_WRITE;
	}

	assert( ( "Unknown BufferObject::MapMode", 0 ) );
	return -1;
}
#pragma endregion

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
#pragma region Texture
// ------------------------------------------------------------------------------------------------
uint32_t glWrap(const TextureFormat &format)
{
	switch( format )
	{
	case TextureFormat::RedGreenBlue8:       return GL_RGB8;
	case TextureFormat::RedGreenBlue16:      return GL_RGB16;
	case TextureFormat::RedGreenBlueAlpha8:  return GL_RGBA8;
	case TextureFormat::RedGreenBlue10A2:    return GL_RGB10_A2;
	case TextureFormat::RedGreenBlueAlpha16: return GL_RGBA16;

	case TextureFormat::Depth16: return GL_DEPTH_COMPONENT16;
	case TextureFormat::Depth24: return GL_DEPTH_COMPONENT24;

	case TextureFormat::Red8:        return GL_R8;
	case TextureFormat::Red16:       return GL_R16;
	case TextureFormat::RedGreen8:   return GL_RG8;
	case TextureFormat::RedGreen16:  return GL_RG16;
	case TextureFormat::Red16f:      return GL_R16F;
	case TextureFormat::Red32f:      return GL_R32F;
	case TextureFormat::RedGreen16f: return GL_RG16F;
	case TextureFormat::RedGreen32f: return GL_RG32F;

	case TextureFormat::Red8i:                 return GL_R8I;
	case TextureFormat::Red8ui:                return GL_R8UI;
	case TextureFormat::Red16i:                return GL_R16I;
	case TextureFormat::Red16ui:               return GL_R16UI;
	case TextureFormat::Red32i:                return GL_R32I;
	case TextureFormat::Red32ui:               return GL_R32UI;
	case TextureFormat::RedGreen8i:            return GL_RG8I;
	case TextureFormat::RedGreen8ui:           return GL_RG8UI;
	case TextureFormat::RedGreen16i:           return GL_RG16I;
	case TextureFormat::RedGreen16ui:          return GL_RG16UI;
	case TextureFormat::RedGreen32i:           return GL_RG32I;
	case TextureFormat::RedGreen32ui:          return GL_RG32UI;
	case TextureFormat::RedGreenBlueAlpha32f:  return GL_RGBA32F;
	case TextureFormat::RedGreenBlue32f:       return GL_RGB32F;
	case TextureFormat::RedGreenBlueAlpha16f:  return GL_RGBA16F;
	case TextureFormat::RedGreenBlue16f:       return GL_RGB16F;
	case TextureFormat::Depth24Stencil8:       return GL_DEPTH24_STENCIL8;
	case TextureFormat::Red11fGreen11fBlue10f: return GL_R11F_G11F_B10F;
	case TextureFormat::RedGreenBlue9E5:       return GL_RGB9_E5;
	case TextureFormat::SRedGreenBlue8:        return GL_SRGB8;
	case TextureFormat::SRedGreenBlue8Alpha8:  return GL_SRGB8_ALPHA8;
	case TextureFormat::Depth32f:              return GL_DEPTH_COMPONENT32F;
	case TextureFormat::Depth32fStencil8:      return GL_DEPTH32F_STENCIL8;

	case TextureFormat::RedGreenBlueAlpha32ui: return GL_RGBA32UI;
	case TextureFormat::RedGreenBlue32ui:      return GL_RGB32UI;
	case TextureFormat::RedGreenBlueAlpha16ui: return GL_RGBA16UI;
	case TextureFormat::RedGreenBlue16ui:      return GL_RGB16UI;
	case TextureFormat::RedGreenBlueAlpha8ui:  return GL_RGBA8UI;
	case TextureFormat::RedGreenBlue8ui:       return GL_RGB8UI;

	case TextureFormat::RedGreenBlueAlpha32i:  return GL_RGBA32I;
	case TextureFormat::RedGreenBlue32i:       return GL_RGB32I;
	case TextureFormat::RedGreenBlueAlpha16i:  return GL_RGBA16I;
	case TextureFormat::RedGreenBlue16i:       return GL_RGB16I;
	case TextureFormat::RedGreenBlueAlpha8i:   return GL_RGBA8I;
	case TextureFormat::RedGreenBlue8i:        return GL_RGB8I;
	}

	assert( ( "Unknown TextureFormat", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrapTextureFormatToPixelType( const TextureFormat &f )
{
	switch( f )
	{
	case TextureFormat::RedGreenBlue8:       return GL_UNSIGNED_BYTE;
	case TextureFormat::RedGreenBlue16:      return GL_UNSIGNED_SHORT;
	case TextureFormat::RedGreenBlueAlpha8:  return GL_UNSIGNED_BYTE;
	case TextureFormat::RedGreenBlue10A2:    return GL_UNSIGNED_INT_10_10_10_2;
	case TextureFormat::RedGreenBlueAlpha16: return GL_UNSIGNED_SHORT;
	case TextureFormat::Depth16:             return GL_HALF_FLOAT;
	case TextureFormat::Depth24:             return GL_FLOAT;
	case TextureFormat::Red8:                return GL_UNSIGNED_BYTE;
	case TextureFormat::Red16:               return GL_UNSIGNED_SHORT;
	case TextureFormat::RedGreen8:           return GL_UNSIGNED_BYTE;
	case TextureFormat::RedGreen16:          return GL_UNSIGNED_SHORT;
	case TextureFormat::Red16f:              return GL_HALF_FLOAT;
	case TextureFormat::Red32f:              return GL_FLOAT;
	case TextureFormat::RedGreen16f:         return GL_HALF_FLOAT;
	case TextureFormat::RedGreen32f:         return GL_FLOAT;

	case TextureFormat::Red8i:   return GL_BYTE;
	case TextureFormat::Red8ui:  return GL_UNSIGNED_BYTE;
	case TextureFormat::Red16i:  return GL_SHORT;
	case TextureFormat::Red16ui: return GL_UNSIGNED_SHORT;
	case TextureFormat::Red32i:  return GL_INT;
	case TextureFormat::Red32ui: return GL_UNSIGNED_INT;

	case TextureFormat::RedGreen8i:   return GL_BYTE;
	case TextureFormat::RedGreen8ui:  return GL_UNSIGNED_BYTE;
	case TextureFormat::RedGreen16i:  return GL_SHORT;
	case TextureFormat::RedGreen16ui: return GL_UNSIGNED_SHORT;
	case TextureFormat::RedGreen32i:  return GL_INT;
	case TextureFormat::RedGreen32ui: return GL_UNSIGNED_INT;

	case TextureFormat::RedGreenBlueAlpha32f:  return GL_FLOAT;
	case TextureFormat::RedGreenBlue32f:       return GL_FLOAT;
	case TextureFormat::RedGreenBlueAlpha16f:  return GL_HALF_FLOAT;
	case TextureFormat::RedGreenBlue16f:       return GL_HALF_FLOAT;
	case TextureFormat::Depth24Stencil8:       return GL_UNSIGNED_INT_24_8;
	case TextureFormat::Red11fGreen11fBlue10f: return GL_FLOAT;
	case TextureFormat::RedGreenBlue9E5:       return GL_FLOAT;
	case TextureFormat::SRedGreenBlue8:        return GL_BYTE;
	case TextureFormat::SRedGreenBlue8Alpha8:  return GL_BYTE;
	case TextureFormat::Depth32f:              return GL_FLOAT;
	case TextureFormat::Depth32fStencil8:      return GL_FLOAT;

	case TextureFormat::RedGreenBlueAlpha32ui: return GL_UNSIGNED_INT;
	case TextureFormat::RedGreenBlue32ui:      return GL_UNSIGNED_INT;
	case TextureFormat::RedGreenBlueAlpha16ui: return GL_UNSIGNED_SHORT;
	case TextureFormat::RedGreenBlue16ui:      return GL_UNSIGNED_SHORT;
	case TextureFormat::RedGreenBlueAlpha8ui:  return GL_UNSIGNED_BYTE;
	case TextureFormat::RedGreenBlue8ui:       return GL_UNSIGNED_BYTE;

	case TextureFormat::RedGreenBlueAlpha32i: return GL_UNSIGNED_INT;
	case TextureFormat::RedGreenBlue32i:      return GL_UNSIGNED_INT;
	case TextureFormat::RedGreenBlueAlpha16i: return GL_UNSIGNED_SHORT;
	case TextureFormat::RedGreenBlue16i:      return GL_UNSIGNED_SHORT;
	case TextureFormat::RedGreenBlueAlpha8i:  return GL_UNSIGNED_BYTE;
	case TextureFormat::RedGreenBlue8i:       return GL_UNSIGNED_BYTE;
	}

	assert( ( "Unknown TextureFormat", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrapTextureFormatToPixelFormat(const TextureFormat &f)
{
	switch (f)
	{
	case TextureFormat::RedGreenBlue8:
	case TextureFormat::RedGreenBlue16: return  GL_RGB;

	case TextureFormat::RedGreenBlueAlpha8:
	case TextureFormat::RedGreenBlue10A2:
	case TextureFormat::RedGreenBlueAlpha16: return  GL_RGBA;

	case TextureFormat::Depth16:
	case TextureFormat::Depth24: return  GL_DEPTH_COMPONENT;

	case TextureFormat::Red8:
	case TextureFormat::Red16: return  GL_RED;

	case TextureFormat::RedGreen8:
	case TextureFormat::RedGreen16: return  GL_RG;

	case TextureFormat::Red16f:
	case TextureFormat::Red32f: return  GL_RED;

	case TextureFormat::RedGreen16f:
	case TextureFormat::RedGreen32f: return  GL_RG;

	case TextureFormat::Red8i:   return  GL_RED_INTEGER;
	case TextureFormat::Red8ui:  return  GL_RED_INTEGER;
	case TextureFormat::Red16i:  return  GL_RED_INTEGER;
	case TextureFormat::Red16ui: return  GL_RED_INTEGER;
	case TextureFormat::Red32i:  return  GL_RED_INTEGER;
	case TextureFormat::Red32ui: return  GL_RED_INTEGER;

	case TextureFormat::RedGreen8i:   return GL_RG_INTEGER;
	case TextureFormat::RedGreen8ui:  return GL_RG_INTEGER;
	case TextureFormat::RedGreen16i:  return GL_RG_INTEGER;
	case TextureFormat::RedGreen16ui: return GL_RG_INTEGER;
	case TextureFormat::RedGreen32i:  return GL_RG_INTEGER;
	case TextureFormat::RedGreen32ui: return GL_RG_INTEGER;

	case TextureFormat::RedGreenBlueAlpha32f: return GL_RGBA;
	case TextureFormat::RedGreenBlue32f:      return GL_RGB;
	case TextureFormat::RedGreenBlueAlpha16f: return GL_RGBA;
	case TextureFormat::RedGreenBlue16f:      return GL_RGB;
	case TextureFormat::Depth24Stencil8:      return GL_DEPTH_STENCIL;

	case TextureFormat::Red11fGreen11fBlue10f:
	case TextureFormat::RedGreenBlue9E5:      return GL_RGB;
	case TextureFormat::SRedGreenBlue8:       return GL_RGB_INTEGER;
	case TextureFormat::SRedGreenBlue8Alpha8: return GL_RGBA_INTEGER;

	case TextureFormat::Depth32f:         return  GL_DEPTH_COMPONENT;
	case TextureFormat::Depth32fStencil8: return  GL_DEPTH_STENCIL;

	case TextureFormat::RedGreenBlueAlpha32ui: return GL_RGBA_INTEGER;
	case TextureFormat::RedGreenBlue32ui:      return GL_RGB_INTEGER;
	case TextureFormat::RedGreenBlueAlpha16ui: return GL_RGBA_INTEGER;
	case TextureFormat::RedGreenBlue16ui:      return GL_RGB_INTEGER;
	case TextureFormat::RedGreenBlueAlpha8ui:  return GL_RGBA_INTEGER;
	case TextureFormat::RedGreenBlue8ui:       return GL_RGB_INTEGER;

	case TextureFormat::RedGreenBlueAlpha32i: return GL_RGBA_INTEGER;
	case TextureFormat::RedGreenBlue32i:      return GL_RGB_INTEGER;
	case TextureFormat::RedGreenBlueAlpha16i: return GL_RGBA_INTEGER;
	case TextureFormat::RedGreenBlue16i:      return GL_RGB_INTEGER;
	case TextureFormat::RedGreenBlueAlpha8i:  return GL_RGBA_INTEGER;
	case TextureFormat::RedGreenBlue8i:       return GL_RGB_INTEGER;
	}
	assert( ( "Unknown TextureFormat", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const Sampler::MinificationFilter &filter )
{
	switch( filter )
	{
	case Sampler::MinificationFilter::Nearest:              return GL_NEAREST;
	case Sampler::MinificationFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
	case Sampler::MinificationFilter::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
	case Sampler::MinificationFilter::Linear:               return GL_LINEAR;
	case Sampler::MinificationFilter::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
	case Sampler::MinificationFilter::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
	}

	assert( ( "Unknown Sampler::MinificationFilter", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const Sampler::MagnificationFilter &filter )
{
	switch( filter )
	{
	case Sampler::MagnificationFilter::Nearest:  return GL_NEAREST;
	case Sampler::MagnificationFilter::Linear:   return GL_LINEAR;
	}

	assert( ( "Unknown Sampler::MagnificationFilter", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const Sampler::Wrap &wrap )
{
	switch( wrap )
	{
	case Sampler::Wrap::Clamp:           return GL_CLAMP_TO_EDGE;
	case Sampler::Wrap::Repeat:          return GL_REPEAT;
	case Sampler::Wrap::MirroredRepeat:  return GL_MIRRORED_REPEAT;
	}

	assert( ( "Unknown Sampler::Wrap", 0 ) );
	return -1;
}
#pragma endregion

/************************************************************************************************/
/*                                           FrameBuffer                                        */
/************************************************************************************************/
#pragma region FrameBuffer
// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const FrameBuffer::AttachmentPoint &attachment )
{
	switch( attachment )
	{
	case FrameBuffer::ColorAttachment0:       return GL_COLOR_ATTACHMENT0;
	case FrameBuffer::ColorAttachment1:       return GL_COLOR_ATTACHMENT1;
	case FrameBuffer::ColorAttachment2:       return GL_COLOR_ATTACHMENT2;
	case FrameBuffer::ColorAttachment3:       return GL_COLOR_ATTACHMENT3;
	case FrameBuffer::ColorAttachment4:       return GL_COLOR_ATTACHMENT4;
	case FrameBuffer::ColorAttachment5:       return GL_COLOR_ATTACHMENT5;
	case FrameBuffer::ColorAttachment6:       return GL_COLOR_ATTACHMENT6;
	case FrameBuffer::ColorAttachment7:       return GL_COLOR_ATTACHMENT7;
	case FrameBuffer::ColorAttachment8:       return GL_COLOR_ATTACHMENT8;
	case FrameBuffer::ColorAttachment9:       return GL_COLOR_ATTACHMENT9;
	case FrameBuffer::DepthAttachment:        return GL_DEPTH_ATTACHMENT;
	case FrameBuffer::DepthStencilAttachment: return GL_DEPTH_STENCIL_ATTACHMENT;
	}

	assert( ( "Unknown FrameBuffer::AttachmentPoint", 0 ) );
	return -1;
}
#pragma endregion

/************************************************************************************************/
/*                                           ImageFormat                                        */
/************************************************************************************************/
#pragma region ImageFormat
// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const ImageFormat &imgFormat )
{
	switch( imgFormat )
	{
    case ImageFormat::StencilIndex:             return GL_STENCIL_INDEX;
    case ImageFormat::DepthComponent:           return GL_DEPTH_COMPONENT;
    case ImageFormat::Red:                      return GL_RED;
    case ImageFormat::Green:                    return GL_GREEN;
    case ImageFormat::Blue:                     return GL_BLUE;
    case ImageFormat::RedGreenBlue:             return GL_RGB;
    case ImageFormat::RedGreenBlueAlpha:        return GL_RGBA;
    case ImageFormat::BlueGreenRed:             return GL_BGR;
    case ImageFormat::BlueGreenRedAlpha:        return GL_BGRA;
    case ImageFormat::RedGreen:                 return GL_RG;
    case ImageFormat::RedGreenInteger:          return GL_RG_INTEGER;
    case ImageFormat::DepthStencil:             return GL_DEPTH_STENCIL;
    case ImageFormat::RedInteger:               return GL_RED_INTEGER;
    case ImageFormat::GreenInteger:             return GL_GREEN_INTEGER;
    case ImageFormat::BlueInteger:              return GL_BLUE_INTEGER;
    case ImageFormat::RedGreenBlueInteger:      return GL_RGB_INTEGER;
    case ImageFormat::RedGreenBlueAlphaInteger: return GL_RGBA_INTEGER;
    case ImageFormat::BlueGreenRedInteger:      return GL_BGR_INTEGER;
	case ImageFormat::BlueGreenRedAlphaInteger: return GL_BGRA_INTEGER;
	}

	assert( ( "Unknown ImageFormat", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const ImageDataType &imgDataType )
{
	switch( imgDataType )
	{
	case ImageDataType::Byte:                          return GL_BYTE;
	case ImageDataType::UnsignedByte:                  return GL_UNSIGNED_BYTE;
	case ImageDataType::Short:                         return GL_SHORT;
	case ImageDataType::UnsignedShort:                 return GL_UNSIGNED_SHORT;
	case ImageDataType::Int:                           return GL_INT;
	case ImageDataType::UnsignedInt:                   return GL_UNSIGNED_INT;
	case ImageDataType::Float:                         return GL_FLOAT;
	case ImageDataType::HalfFloat:                     return GL_HALF_FLOAT;
	case ImageDataType::UnsignedByte332:               return GL_UNSIGNED_BYTE_3_3_2;
	case ImageDataType::UnsignedShort4444:             return GL_UNSIGNED_SHORT_4_4_4_4;
	case ImageDataType::UnsignedShort5551:             return GL_UNSIGNED_SHORT_5_5_5_1;
	case ImageDataType::UnsignedInt8888:               return GL_UNSIGNED_INT_8_8_8_8;
	case ImageDataType::UnsignedInt1010102:            return GL_UNSIGNED_INT_10_10_10_2;
	case ImageDataType::UnsignedByte233Reversed:       return GL_UNSIGNED_BYTE_2_3_3_REV;
	case ImageDataType::UnsignedShort565:              return GL_UNSIGNED_SHORT_5_6_5;
	case ImageDataType::UnsignedShort565Reversed:      return GL_UNSIGNED_SHORT_5_6_5_REV;
	case ImageDataType::UnsignedShort4444Reversed:     return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	case ImageDataType::UnsignedShort1555Reversed:     return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case ImageDataType::UnsignedInt8888Reversed:       return GL_UNSIGNED_INT_8_8_8_8_REV;
	case ImageDataType::UnsignedInt2101010Reversed:    return GL_UNSIGNED_INT_2_10_10_10_REV;
	case ImageDataType::UnsignedInt248:                return GL_UNSIGNED_INT_24_8;
	case ImageDataType::UnsignedInt10F11F11FReversed:  return GL_UNSIGNED_INT_10F_11F_11F_REV;
	case ImageDataType::UnsignedInt5999Reversed:       return GL_UNSIGNED_INT_5_9_9_9_REV;
	case ImageDataType::Float32UnsignedInt248Reversed: return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
	}

	assert( ( "Unknown ImageDataType", 0 ) );
	return -1;
}
#pragma endregion
/************************************************************************************************/
/*                                           RenderBuffer                                       */
/************************************************************************************************/
#pragma region RenderBuffer
// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const RenderBuffer::Format &format )
{
	switch( format )
	{
	case RenderBuffer::Format::R8:                return GL_R8;                
    case RenderBuffer::Format::R8_SNORM:          return GL_R8_SNORM;          
    case RenderBuffer::Format::R16:               return GL_R16;               
    case RenderBuffer::Format::R16_SNORM:         return GL_R16_SNORM;         
    case RenderBuffer::Format::RG8:               return GL_RG8;               
    case RenderBuffer::Format::RG8_SNORM:         return GL_RG8_SNORM;         
    case RenderBuffer::Format::RG16:              return GL_RG16;              
    case RenderBuffer::Format::RG16_SNORM:        return GL_RG16_SNORM;        
    case RenderBuffer::Format::R3_G3_B2:          return GL_R3_G3_B2;          
    case RenderBuffer::Format::RGB4:              return GL_RGB4;              
    case RenderBuffer::Format::RGB5:              return GL_RGB5;              
    case RenderBuffer::Format::RGB8:              return GL_RGB8;              
    case RenderBuffer::Format::RGB8_SNORM:        return GL_RGB8_SNORM;        
    case RenderBuffer::Format::RGB10:             return GL_RGB10;             
    case RenderBuffer::Format::RGB12:             return GL_RGB12;             
    case RenderBuffer::Format::RGB16:             return GL_RGB16;             
    case RenderBuffer::Format::RGB16_SNORM:       return GL_RGB16_SNORM;       
    case RenderBuffer::Format::RGBA2:             return GL_RGBA2;             
    case RenderBuffer::Format::RGBA4:             return GL_RGBA4;             
    case RenderBuffer::Format::RGB5_A1:           return GL_RGB5_A1;           
    case RenderBuffer::Format::RGBA8:             return GL_RGBA8;             
    case RenderBuffer::Format::RGBA8_SNORM:       return GL_RGBA8_SNORM;       
    case RenderBuffer::Format::RGB10_A2:          return GL_RGB10_A2;          
    case RenderBuffer::Format::RGB10_A2UI:        return GL_RGB10_A2UI;        
    case RenderBuffer::Format::RGBA12:            return GL_RGBA12;            
    case RenderBuffer::Format::RGBA16:            return GL_RGBA16;            
    case RenderBuffer::Format::RGBA16_SNORM:      return GL_RGBA16_SNORM;      
    case RenderBuffer::Format::SRGB8:             return GL_SRGB8;             
    case RenderBuffer::Format::SRGB8_ALPHA8:      return GL_SRGB8_ALPHA8;      
    case RenderBuffer::Format::R16F:              return GL_R16F;              
    case RenderBuffer::Format::RG16F:             return GL_RG16F;             
    case RenderBuffer::Format::RGB16F:            return GL_RGB16F;            
    case RenderBuffer::Format::RGBA16F:           return GL_RGBA16F;           
    case RenderBuffer::Format::R32F:              return GL_R32F;              
    case RenderBuffer::Format::RG32F:             return GL_RG32F;             
    case RenderBuffer::Format::RGB32F:            return GL_RGB32F;            
    case RenderBuffer::Format::RGBA32F:           return GL_RGBA32F;           
    case RenderBuffer::Format::R11F_G11F_B10F:    return GL_R11F_G11F_B10F;    
    case RenderBuffer::Format::RGB9_E5:           return GL_RGB9_E5;           
    case RenderBuffer::Format::R8I:               return GL_R8I;               
    case RenderBuffer::Format::R8UI:              return GL_R8UI;              
    case RenderBuffer::Format::R16I:              return GL_R16I;              
    case RenderBuffer::Format::R16UI:             return GL_R16UI;             
    case RenderBuffer::Format::R32I:              return GL_R32I;              
    case RenderBuffer::Format::R32UI:             return GL_R32UI;             
    case RenderBuffer::Format::RG8I:              return GL_RG8I;              
    case RenderBuffer::Format::RG8UI:             return GL_RG8UI;             
    case RenderBuffer::Format::RG16I:             return GL_RG16I;             
    case RenderBuffer::Format::RG16UI:            return GL_RG16UI;            
    case RenderBuffer::Format::RG32I:             return GL_RG32I;             
    case RenderBuffer::Format::RG32UI:            return GL_RG32UI;            
    case RenderBuffer::Format::RGB8I:             return GL_RGB8I;             
    case RenderBuffer::Format::RGB8UI:            return GL_RGB8UI;            
    case RenderBuffer::Format::RGB16I:            return GL_RGB16I;            
    case RenderBuffer::Format::RGB16UI:           return GL_RGB16UI;           
    case RenderBuffer::Format::RGB32I:            return GL_RGB32I;            
    case RenderBuffer::Format::RGB32UI:           return GL_RGB32UI;           
    case RenderBuffer::Format::RGBA8I:            return GL_RGBA8I;            
    case RenderBuffer::Format::RGBA8UI:           return GL_RGBA8UI;           
    case RenderBuffer::Format::RGBA16I:           return GL_RGBA16I;           
    case RenderBuffer::Format::RGBA16UI:          return GL_RGBA16UI;          
    case RenderBuffer::Format::RGBA32I:           return GL_RGBA32I;           
    case RenderBuffer::Format::RGBA32UI:          return GL_RGBA32UI;          
    case RenderBuffer::Format::DepthComponent:    return GL_DEPTH_COMPONENT;    
    case RenderBuffer::Format::DepthComponent16:  return GL_DEPTH_COMPONENT16;  
    case RenderBuffer::Format::DepthComponent24:  return GL_DEPTH_COMPONENT24;  
    case RenderBuffer::Format::DepthComponent32:  return GL_DEPTH_COMPONENT32;  
    case RenderBuffer::Format::DepthComponent32F: return GL_DEPTH_COMPONENT32F; 
    case RenderBuffer::Format::Depth24Stencil8:   return GL_DEPTH24_STENCIL8;
    case RenderBuffer::Format::Depth32FStencil8:  return GL_DEPTH32F_STENCIL8;
    case RenderBuffer::Format::StencilIndex:      return GL_STENCIL_INDEX;  
    case RenderBuffer::Format::StencilIndex1:     return GL_STENCIL_INDEX1;
    case RenderBuffer::Format::StencilIndex4:     return GL_STENCIL_INDEX4;
	case RenderBuffer::Format::StencilIndex8:     return GL_STENCIL_INDEX8;
    case RenderBuffer::Format::StencilIndex16:    return GL_STENCIL_INDEX16;
	}

	assert( ( "Unknown RenderBuffer", 0 ) );
	return -1;
}
#pragma endregion

/************************************************************************************************/
/*                                           RenderState                                        */
/************************************************************************************************/
#pragma region RenderState
// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const FaceCulling::Face &face )
{
	switch( face )
	{
	case FaceCulling::Face::Front:        return GL_FRONT;
	case FaceCulling::Face::Back:         return GL_BACK;
	case FaceCulling::Face::FrontAndBack: return GL_FRONT_AND_BACK;
	}
	assert( ( "Unknown FaceCulling::Face", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const FaceCulling::WindingOrder &winding )
{
	switch( winding )
	{
	case FaceCulling::WindingOrder::ClockWise:        return GL_CW;
	case FaceCulling::WindingOrder::CounterClockWise: return GL_CCW;
	}
	assert( ( "Unknown FaceCulling::WindingOrder", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const StencilTestFace::StencilOperation &op )
{
	switch( op )
	{
	case StencilTestFace::StencilOperation::Zero:          return GL_ZERO;
	case StencilTestFace::StencilOperation::Invert:        return GL_INVERT;
	case StencilTestFace::StencilOperation::Keep:          return GL_KEEP;
	case StencilTestFace::StencilOperation::Replace:       return GL_REPLACE;
	case StencilTestFace::StencilOperation::Increment:     return GL_INCR;
	case StencilTestFace::StencilOperation::Decrement:     return GL_DECR;
	case StencilTestFace::StencilOperation::IncrementWrap: return GL_INCR_WRAP;
	case StencilTestFace::StencilOperation::DecrementWrap: return GL_DECR_WRAP;
	}
	assert( ( "Unknown StencilTestFace::StencilOperation", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const StencilTestFace::StencilTestFunction &fun )
{
	switch( fun )
	{
	case StencilTestFace::StencilTestFunction::Never:              return GL_NEVER;
	case StencilTestFace::StencilTestFunction::Less:               return GL_LESS;
	case StencilTestFace::StencilTestFunction::Equal:              return GL_EQUAL;
	case StencilTestFace::StencilTestFunction::LessThanOrEqual:    return GL_LEQUAL;
	case StencilTestFace::StencilTestFunction::Greater:            return GL_GREATER;
	case StencilTestFace::StencilTestFunction::NotEqual:           return GL_NOTEQUAL;
	case StencilTestFace::StencilTestFunction::GreaterThanOrEqual: return GL_GEQUAL;
	case StencilTestFace::StencilTestFunction::Always:             return GL_ALWAYS;
	}
	assert( ( "Unknown StencilTestFace::StencilTestFunction", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const DepthTest::DepthTestFunction &fun )
{
	switch( fun )
	{
	case DepthTest::DepthTestFunction::Never:              return GL_NEVER;
	case DepthTest::DepthTestFunction::Less:               return GL_LESS;
	case DepthTest::DepthTestFunction::Equal:              return GL_EQUAL;
	case DepthTest::DepthTestFunction::LessThanOrEqual:    return GL_LEQUAL;
	case DepthTest::DepthTestFunction::Greater:            return GL_GREATER;
	case DepthTest::DepthTestFunction::NotEqual:           return GL_NOTEQUAL;
	case DepthTest::DepthTestFunction::GreaterThanOrEqual: return GL_GEQUAL;
	case DepthTest::DepthTestFunction::Always:             return GL_ALWAYS;
	}
	assert( ( "Unknown DepthTest::DepthTestFunction", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const Blending::Factor &factor )
{
	switch( factor )
	{
	case Blending::Factor::Zero:                     return GL_ZERO;
	case Blending::Factor::One:                      return GL_ONE;
	case Blending::Factor::SourceAlpha:              return GL_SRC_ALPHA;
	case Blending::Factor::OneMinusSourceAlpha:      return GL_ONE_MINUS_SRC_ALPHA;
	case Blending::Factor::SourceColor:              return GL_SRC_COLOR;
	case Blending::Factor::OneMinusSourceColor:      return GL_ONE_MINUS_SRC_COLOR;
	case Blending::Factor::DestinationAlpha:         return GL_DST_ALPHA;
	case Blending::Factor::OneMinusDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
	case Blending::Factor::DestinationColor:         return GL_DST_COLOR;
	case Blending::Factor::OneMinusDestinationColor: return GL_ONE_MINUS_DST_COLOR;
	case Blending::Factor::SourceAlphaSaturate:      return GL_SRC_ALPHA_SATURATE;
	case Blending::Factor::ConstantColor:            return GL_CONSTANT_COLOR;
	case Blending::Factor::OneMinusConstantColor:    return GL_ONE_MINUS_CONSTANT_COLOR;
	case Blending::Factor::ConstantAlpha:            return GL_CONSTANT_ALPHA;
	case Blending::Factor::OneMinusConstantAlpha:    return GL_ONE_MINUS_CONSTANT_ALPHA;
	}
	assert( ( "Unknown Blending::Factor", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const Blending::Equation &equation )
{
	switch( equation )
	{
	case Blending::Equation::Add:             return GL_FUNC_ADD;
	case Blending::Equation::Minimum:         return GL_MIN;
	case Blending::Equation::Maximum:         return GL_MAX;
	case Blending::Equation::Subtract:        return GL_FUNC_SUBTRACT;
	case Blending::Equation::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
	}
	assert( ( "Unknown Blending::BlendEquation", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const ClearBuffers &buffers )
{
	switch( buffers )
	{
	case ClearBuffers::ColorBuffer:           return GL_COLOR_BUFFER_BIT;
	case ClearBuffers::DepthBuffer:           return GL_DEPTH_BUFFER_BIT;
	case ClearBuffers::StencilBuffer:         return GL_STENCIL_BUFFER_BIT;
	case ClearBuffers::ColorAndDepthBuffer:   return GL_COLOR_BUFFER_BIT   | GL_DEPTH_BUFFER_BIT;
	case ClearBuffers::ColorAndStencilBuffer: return GL_COLOR_BUFFER_BIT   | GL_STENCIL_BUFFER_BIT;
	case ClearBuffers::StencilAndDepthBuffer: return GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	case ClearBuffers::All:                   return GL_COLOR_BUFFER_BIT   | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT;
	}
	assert( ( "Unknown ClearBuffers", 0 ) );
	return -1;
}

// ------------------------------------------------------------------------------------------------
uint32_t glWrap( const RenderState::RasterizationMode &mode )
{
	switch( mode )
	{
	case RenderState::RasterizationMode::Point: return GL_POINT;
	case RenderState::RasterizationMode::Line:  return GL_LINE;
	case RenderState::RasterizationMode::Fill:  return GL_FILL;
	}
	assert( ( "Unknown RenderState::RasterizationMode", 0 ) );
	return -1;
}

#pragma endregion

/************************************************************************************************/
/*                                      PrimitiveType                                           */
/************************************************************************************************/
#pragma region PrimitiveType
uint32_t glWrap( const PrimitiveType &primitive )
{
	switch( primitive )
	{
	case PrimitiveType::Points:        return GL_POINTS;
	case PrimitiveType::Lines:         return GL_LINES;
	case PrimitiveType::LineStrip:     return GL_LINE_STRIP;
	case PrimitiveType::LineLoop:      return GL_LINE_LOOP;
	case PrimitiveType::Triangles:     return GL_TRIANGLES;
	case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
	case PrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
	case PrimitiveType::Quads:         return GL_QUADS;
	case PrimitiveType::QuadStrip:     return GL_QUAD_STRIP;
	}
	assert( ( "Unknown PrimitiveType", 0 ) );
	return -1;
}

#pragma endregion

}