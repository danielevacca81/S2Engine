// OpenGLWrap.h
// 
#ifndef OPENGLWRAP_ONCE
#define OPENGLWRAP_ONCE

#include "Texture.h"
#include "Sampler.h"
#include "FrameBuffer.h"
#include "VertexBuffer.h"
#include "AttributeBuffer.h"
#include "IndexBuffer.h"
#include "RenderState.h"
#include "RenderBuffer.h"
#include "PrimitiveType.h"

#define DATA_PTR(i) ((void*)(i))


namespace RenderSystem {

/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
uint32_t glWrap( const IndexBuffer::IndexDataType &datatype );

/************************************************************************************************/
/*                                         AttributeBuffer                                      */
/************************************************************************************************/
uint32_t glWrap( const AttributeBuffer::ComponentDatatype &datatype );

/************************************************************************************************/
/*                                           BufferObject                                       */
/************************************************************************************************/
uint32_t glWrap( const BufferObject::UsageHint &hint );
uint32_t glWrap( const BufferObject::Type      &type );
uint32_t glWrap( const BufferObject::MapMode   &mode );

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
uint32_t glWrap( const TextureFormat             &format );
uint32_t glWrapTextureFormatToPixelFormat(const TextureFormat &f);
uint32_t glWrapTextureFormatToPixelType(const TextureFormat &f);
uint32_t glWrap( const Sampler::MinificationFilter &filter );
uint32_t glWrap( const Sampler::MagnificationFilter &filter );
uint32_t glWrap( const Sampler::Wrap        &wrap );

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
/*                                           RenderBuffer                                       */
/************************************************************************************************/
uint32_t glWrap( const RenderBuffer::Format &attachment );

/************************************************************************************************/
/*                                           RenderState                                        */
/************************************************************************************************/
uint32_t glWrap( const FaceCulling::Face                    &face );
uint32_t glWrap( const FaceCulling::WindingOrder            &winding );
uint32_t glWrap( const StencilTestFace::StencilOperation    &op );
uint32_t glWrap( const StencilTestFace::StencilTestFunction &fun );
uint32_t glWrap( const DepthTest::DepthTestFunction         &winding );
uint32_t glWrap( const Blending::Factor                     &factor );
uint32_t glWrap( const Blending::Equation                   &equation );
uint32_t glWrap( const ClearBuffers                         &buffers );
uint32_t glWrap( const RenderState::RasterizationMode       &mode );

/************************************************************************************************/
/*                                            Primitive                                         */
/************************************************************************************************/
uint32_t glWrap( const PrimitiveType &primitive );

}
#endif