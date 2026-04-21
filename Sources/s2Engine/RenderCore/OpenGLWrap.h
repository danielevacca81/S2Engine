// OpenGLWrap.h
// 
#ifndef S2_RENDERCORE_OPENGLWRAP_H
#define S2_RENDERCORE_OPENGLWRAP_H

#include "Texture.h"
#include "GPUBufferObject.h"
#include "FrameBuffer.h"
#include "AttributeBuffer.h"
#include "IndexBuffer.h"
#include "RenderState.h"
#include "PrimitiveType.h"
#include "ShaderStage.h"


namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
uint32_t glWrap( const IndexBuffer::IndexDataType &datatype );

/************************************************************************************************/
/*                                         AttributeBuffer                                      */
/************************************************************************************************/
uint32_t glWrap( const AttributeBuffer::ComponentDatatype &datatype );

/************************************************************************************************/
/*                                           GPUBufferObject                                       */
/************************************************************************************************/
uint32_t glWrap( const GPUBufferObject::UsageHint &hint );
uint32_t glWrap( const GPUBufferObject::Type      &type );
uint32_t glWrap( const GPUBufferObject::MapMode   &mode );

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
uint32_t glWrap( const TextureFormat             &format );
uint32_t glWrapTextureFormatToPixelFormat(const TextureFormat &f);
uint32_t glWrapTextureFormatToPixelType(const TextureFormat &f);
uint32_t glWrap( const Texture2D::MinFilter &filter );
uint32_t glWrap( const Texture2D::MagFilter &filter );
uint32_t glWrap( const Texture2D::WrapMode &wrap );

/************************************************************************************************/
/*                                           ImageFormat                                        */
/************************************************************************************************/
uint32_t glWrap( const ImageFormat &imgFormat );
uint32_t glWrap( const ImageDataType &imgDataType );

/************************************************************************************************/
/*                                           FrameBuffer                                        */
/************************************************************************************************/
uint32_t glWrap( const FrameBuffer::AttachmentPoint &attachment );

/************************************************************************************************/
/*                                           RenderState                                        */
/************************************************************************************************/
uint32_t glWrap( const FaceCulling::Face                    &face );
uint32_t glWrap( const FaceCulling::WindingOrder            &winding );
uint32_t glWrap( const StencilTestFace::StencilOperation    &op );
uint32_t glWrap( const StencilTestFace::StencilTestFunction &fun );
uint32_t glWrap( const DepthTest::Function                  &function );
uint32_t glWrap( const Blending::Factor                     &factor );
uint32_t glWrap( const Blending::Equation                   &equation );
uint32_t glWrap( const ClearBuffers                         &buffers );
uint32_t glWrap( const RenderState::RasterizationMode       &mode );

/************************************************************************************************/
/*                                            Primitive                                         */
/************************************************************************************************/
uint32_t glWrap( const PrimitiveType &primitive );


/************************************************************************************************/
/*                                            ShaderType                                        */
/************************************************************************************************/
uint32_t glWrap( const ShaderStageType& shaderType );

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_OPENGLWRAP_H