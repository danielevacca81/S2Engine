// OpenGLWrap.cpp
// 
#include "OpenGLWrap.h"

namespace s2 {

namespace OpenGL {
/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
#pragma region
// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const IndexBuffer::IndexDataType &datatype )
{
	switch( datatype )
	{
	case IndexBuffer::UnsignedInt:    return GL_UNSIGNED_INT;
	case IndexBuffer::UnsignedShort:  return GL_UNSIGNED_SHORT;
	}

	return -1;
}
#pragma endregion


/************************************************************************************************/
/*                                         AttributeBuffer                                      */
/************************************************************************************************/
#pragma region
// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const AttributeBuffer::ComponentDatatype &datatype )
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

	return -1;
}
#pragma endregion

/************************************************************************************************/
/*                                        VertexBufferObject                                    */
/************************************************************************************************/
#pragma region
// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const BufferObject::BufferUsageHint &hint )
{
	switch( hint )
	{
	case BufferObject::StreamDraw:  return GL_STREAM_DRAW;
	case BufferObject::StreamRead:  return GL_STREAM_READ;
	case BufferObject::StreamCopy:  return GL_STREAM_COPY;
	case BufferObject::StaticDraw:  return GL_STATIC_DRAW;
	case BufferObject::StaticRead:  return GL_STATIC_READ;
	case BufferObject::StaticCopy:  return GL_STATIC_COPY;
	case BufferObject::DynamicDraw: return GL_DYNAMIC_DRAW;
	case BufferObject::DynamicRead: return GL_DYNAMIC_READ;
	case BufferObject::DynamicCopy: return GL_DYNAMIC_COPY;
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const BufferObject::BufferType &type )
{
	switch( type )
	{
	case BufferObject::ArrayBuffer:    return GL_ARRAY_BUFFER;
	case BufferObject::ElementBuffer:  return GL_ELEMENT_ARRAY_BUFFER;
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const BufferObject::BufferMapMode &mode )
{
	switch( mode )
	{
	case BufferObject::ReadOnly:  return GL_READ_ONLY;
	case BufferObject::WriteOnly: return GL_WRITE_ONLY;
	case BufferObject::ReadWrite: return GL_READ_WRITE;
	}

	return -1;
}
#pragma endregion

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
#pragma region Texture
// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const Texture::TextureType &type )
{
	switch( type )
	{
	case Texture::Texture_1D: return GL_TEXTURE_1D;
	case Texture::Texture_2D: return GL_TEXTURE_1D;
	}

	return GL_INVALID_ENUM;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const Texture::DataFormat &format )
{
	switch( format )
	{
	case Texture::Red: return GL_RED;
	case Texture::RG: return GL_RG;
	case Texture::RGB: return GL_RGB;
	case Texture::BGR: return GL_BGR;
	case Texture::RGBA: return GL_RGBA;
	case Texture::BGRA: return GL_BGRA;
	case Texture::RedInteger: return GL_RED_INTEGER;
	case Texture::RGInteger: return GL_RG_INTEGER;
	case Texture::RGBInteger: return GL_RGB_INTEGER;
	case Texture::BGRInteger: return GL_BGR_INTEGER;
	case Texture::RGBAInteger: return GL_RGBA_INTEGER;
	case Texture::BGRAInteger: return GL_BGRA_INTEGER;
	case Texture::StencilIndex: return GL_STENCIL_INDEX;
	case Texture::DepthComponent: return GL_DEPTH_COMPONENT;
	case Texture::DepthStencil: return GL_DEPTH_STENCIL;
	}

	return -1;
}
// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const Texture::DataType &type )
{
	switch( type )
	{
	case Texture::UByte_8:           return GL_UNSIGNED_BYTE;
	case Texture::Byte_8:            return GL_BYTE;
	case Texture::UShort_16:         return GL_UNSIGNED_SHORT;
	case Texture::Short_16:          return GL_SHORT;
	case Texture::UInt_32:           return GL_UNSIGNED_INT;
	case Texture::Int_32:            return GL_INT;
	case Texture::Float_32:          return GL_FLOAT;
	case Texture::UByte_3_3_2:       return GL_UNSIGNED_BYTE_3_3_2;
	case Texture::UByte_2_3_3_r:     return GL_UNSIGNED_BYTE_2_3_3_REV;
	case Texture::UShort_5_6_5:      return GL_UNSIGNED_SHORT_5_6_5;
	case Texture::UShort_5_6_5_r:    return GL_UNSIGNED_SHORT_5_6_5_REV;
	case Texture::UShort_4_4_4_4:    return GL_UNSIGNED_SHORT_4_4_4_4;
	case Texture::UShort_4_4_4_4_r:  return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	case Texture::UShort_5_5_5_1:    return GL_UNSIGNED_SHORT_5_5_5_1;
	case Texture::UShort_1_5_5_5_r:  return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case Texture::UInt_8_8_8_8:      return GL_UNSIGNED_INT_8_8_8_8;
	case Texture::UInt_8_8_8_8_r:    return GL_UNSIGNED_INT_8_8_8_8_REV;
	case Texture::UInt_10_10_10_2:   return GL_UNSIGNED_INT_10_10_10_2;
	case Texture::UInt_2_10_10_10_r: return GL_UNSIGNED_INT_2_10_10_10_REV;
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const Texture::TextureMinFilter &filter )
{
	switch( filter )
	{
	case Texture::MinFilterNearest:                    return GL_NEAREST;
	case Texture::MinFilterNearestWithMipmaps:         return GL_NEAREST_MIPMAP_NEAREST;
	case Texture::MinFilterNearestWithBilinearMipmaps: return GL_NEAREST_MIPMAP_LINEAR;
	case Texture::MinFilterBilinear:                   return GL_LINEAR;
	case Texture::MinFilterBilinearWithMipmaps:        return GL_LINEAR_MIPMAP_NEAREST;
	case Texture::MinFilterTrilinear:                  return GL_LINEAR_MIPMAP_LINEAR;
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const Texture::TextureMagFilter &filter )
{
	switch( filter )
	{
	case Texture::MagFilterNearest:  return GL_NEAREST;
	case Texture::MagFilterBilinear: return GL_LINEAR;
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const Texture::WrapMode &mode )
{
	switch( mode )
	{
	case Texture::Clamp:           return GL_CLAMP_TO_EDGE;
	case Texture::Repeat:          return GL_REPEAT;
	case Texture::MirroredRepeat:  return GL_MIRRORED_REPEAT;
	}

	return -1;
}
#pragma endregion

/************************************************************************************************/
/*                                           FrameBuffer                                        */
/************************************************************************************************/
#pragma region FrameBuffer
// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const FrameBuffer::AttachmentPoint &attachment )
{
	switch( attachment )
	{
	case FrameBuffer::ColorAttachment0: return GL_COLOR_ATTACHMENT0;
	case FrameBuffer::ColorAttachment1: return GL_COLOR_ATTACHMENT1;
	case FrameBuffer::ColorAttachment2: return GL_COLOR_ATTACHMENT2;
	case FrameBuffer::ColorAttachment3: return GL_COLOR_ATTACHMENT3;
	case FrameBuffer::ColorAttachment4: return GL_COLOR_ATTACHMENT4;
	case FrameBuffer::ColorAttachment5: return GL_COLOR_ATTACHMENT5;
	case FrameBuffer::ColorAttachment6: return GL_COLOR_ATTACHMENT6;
	case FrameBuffer::ColorAttachment7: return GL_COLOR_ATTACHMENT7;
	case FrameBuffer::ColorAttachment8: return GL_COLOR_ATTACHMENT8;
	case FrameBuffer::ColorAttachment9: return GL_COLOR_ATTACHMENT9;
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const FrameBuffer::DepthFormat     &format )
{
	switch( format )
	{
	case FrameBuffer::DepthNone:          return GL_NONE;
	case FrameBuffer::DepthComponent16:   return GL_DEPTH_COMPONENT16;
	case FrameBuffer::DepthComponent24:   return GL_DEPTH_COMPONENT24;
	case FrameBuffer::DepthComponent32:   return GL_DEPTH_COMPONENT32;
	case FrameBuffer::DepthComponent32FF: return GL_DEPTH_COMPONENT32F;
	}
	return -1;
}

#pragma endregion

/************************************************************************************************/
/*                                           RenderState                                        */
/************************************************************************************************/
#pragma region RenderState
// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const FaceCulling::Face &face )
{
	switch( face )
	{
	case FaceCulling::Front:        return GL_FRONT;
	case FaceCulling::Back:         return GL_BACK;
	case FaceCulling::FrontAndBack: return GL_FRONT_AND_BACK;
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const FaceCulling::WindingOrder &winding )
{
	switch( winding )
	{
	case FaceCulling::ClockWise:        return GL_CW;
	case FaceCulling::CounterClockWise: return GL_CCW;
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const StencilTestFace::StencilOperation &op )
{
	switch( op )
	{
	case StencilTestFace::Zero:          return GL_ZERO;
	case StencilTestFace::Invert:        return GL_INVERT;
	case StencilTestFace::Keep:          return GL_KEEP;
	case StencilTestFace::Replace:       return GL_REPLACE;
	case StencilTestFace::Increment:     return GL_INCR;
	case StencilTestFace::Decrement:     return GL_DECR;
	case StencilTestFace::IncrementWrap: return GL_INCR_WRAP;
	case StencilTestFace::DecrementWrap: return GL_DECR_WRAP;
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const StencilTestFace::StencilTestFunction &fun )
{
	switch( fun )
	{
	case StencilTestFace::Never:              return GL_NEVER;
	case StencilTestFace::Less:               return GL_LESS;
	case StencilTestFace::Equal:              return GL_EQUAL;
	case StencilTestFace::LessThanOrEqual:    return GL_LEQUAL;
	case StencilTestFace::Greater:            return GL_GREATER;
	case StencilTestFace::NotEqual:           return GL_NOTEQUAL;
	case StencilTestFace::GreaterThanOrEqual: return GL_GEQUAL;
	case StencilTestFace::Always:             return GL_ALWAYS;
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const DepthTest::DepthTestFunction &fun )
{
	switch( fun )
	{
	case DepthTest::Never:              return GL_NEVER;
	case DepthTest::Less:               return GL_LESS;
	case DepthTest::Equal:              return GL_EQUAL;
	case DepthTest::LessThanOrEqual:    return GL_LEQUAL;
	case DepthTest::Greater:            return GL_GREATER;
	case DepthTest::NotEqual:           return GL_NOTEQUAL;
	case DepthTest::GreaterThanOrEqual: return GL_GEQUAL;
	case DepthTest::Always:             return GL_ALWAYS;
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const Blending::BlendingFactor &factor )
{
	switch( factor )
	{
	case Blending::Zero:                     return GL_ZERO;
	case Blending::One:                      return GL_ONE;
	case Blending::SourceAlpha:              return GL_SRC_ALPHA;
	case Blending::OneMinusSourceAlpha:      return GL_ONE_MINUS_SRC_ALPHA;
	case Blending::SourceColor:              return GL_SRC_COLOR;
	case Blending::OneMinusSourceColor:      return GL_ONE_MINUS_SRC_COLOR;
	case Blending::DestinationAlpha:         return GL_DST_ALPHA;
	case Blending::OneMinusDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
	case Blending::DestinationColor:         return GL_DST_COLOR;
	case Blending::OneMinusDestinationColor: return GL_ONE_MINUS_DST_COLOR;
	case Blending::SourceAlphaSaturate:      return GL_SRC_ALPHA_SATURATE;
	case Blending::ConstantColor:            return GL_CONSTANT_COLOR;
	case Blending::OneMinusConstantColor:    return GL_ONE_MINUS_CONSTANT_COLOR;
	case Blending::ConstantAlpha:            return GL_CONSTANT_ALPHA;
	case Blending::OneMinusConstantAlpha:    return GL_ONE_MINUS_CONSTANT_ALPHA;
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const Blending::BlendEquation &equation )
{
	switch( equation )
	{
	case Blending::Add:             return GL_FUNC_ADD;
	case Blending::Minimum:         return GL_MIN;
	case Blending::Maximum:         return GL_MAX;
	case Blending::Subtract:        return GL_FUNC_SUBTRACT;
	case Blending::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const ClearBuffers &buffers )
{
	switch( buffers )
	{
	case ColorBuffer:         return GL_COLOR_BUFFER_BIT;
	case DepthBuffer:         return GL_DEPTH_BUFFER_BIT;
	case StencilBuffer:       return GL_STENCIL_BUFFER_BIT;
	case ColorAndDepthBuffer: return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	case All:                 return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
	}
	return -1;
}

// ------------------------------------------------------------------------------------------------
unsigned int glWrap( const RenderState::RasterizationMode &mode )
{
	switch( mode )
	{
	case RenderState::Point: return GL_POINT;
	case RenderState::Line:  return GL_LINE;
	case RenderState::Fill:  return GL_FILL;
	}
	return -1;
}

#pragma endregion

/************************************************************************************************/
/*                                        PrimitiveType                                         */
/************************************************************************************************/
#pragma region PrimitiveType
unsigned int glWrap( const PrimitiveType &primitive )
{
	switch( primitive )
	{
	case Points:        return GL_POINTS;
	case Lines:         return GL_LINES;
	case LineStrip:     return GL_LINE_STRIP;
	case LineLoop:      return GL_LINE_LOOP;
	case Triangles:     return GL_TRIANGLES;
	case TriangleStrip: return GL_TRIANGLE_STRIP;
	case TriangleFan:   return GL_TRIANGLE_FAN;
	case Quads:         return GL_QUADS;
	case QuadStrip:     return GL_QUAD_STRIP;
	}
	return -1;
}

#pragma endregion

}
}