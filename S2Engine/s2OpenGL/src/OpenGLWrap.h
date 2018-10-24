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
#include "Primitive.h"

#define BUFFER_OFFSET(i) ((void*)(i))


namespace s2 {
namespace OpenGL {

/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
unsigned int glWrap( const IndexBuffer::IndexDataType &datatype );

/************************************************************************************************/
/*                                         AttributeBuffer                                      */
/************************************************************************************************/
unsigned int glWrap( const AttributeBuffer::ComponentDatatype &datatype );

/************************************************************************************************/
/*                                           BufferObject                                       */
/************************************************************************************************/
unsigned int glWrap( const BufferObject::UsageHint &hint );
unsigned int glWrap( const BufferObject::Type      &type );
unsigned int glWrap( const BufferObject::MapMode   &mode );

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
unsigned int glWrap( const TextureFormat             &format );
unsigned int glWrapTextureFormatToPixelFormat(const TextureFormat &f);
unsigned int glWrapTextureFormatToPixelType(const TextureFormat &f);
unsigned int glWrap( const Sampler::MinificationFilter &filter );
unsigned int glWrap( const Sampler::MagnificationFilter &filter );
unsigned int glWrap( const Sampler::Wrap        &wrap );

/************************************************************************************************/
/*                                           ImageFormat                                        */
/************************************************************************************************/
unsigned int glWrap( const ImageFormat &imgFormat );
unsigned int glWrap( const ImageDataType &imgDataType );

/************************************************************************************************/
/*                                           FrameBuffer                                        */
/************************************************************************************************/
unsigned int glWrap( const FrameBuffer::AttachmentPoint &attachment );

/************************************************************************************************/
/*                                           RenderBuffer                                       */
/************************************************************************************************/
unsigned int glWrap( const RenderBuffer::Format &attachment );

/************************************************************************************************/
/*                                           RenderState                                        */
/************************************************************************************************/
unsigned int glWrap( const FaceCulling::Face                    &face );
unsigned int glWrap( const FaceCulling::WindingOrder            &winding );
unsigned int glWrap( const StencilTestFace::StencilOperation    &op );
unsigned int glWrap( const StencilTestFace::StencilTestFunction &fun );
unsigned int glWrap( const DepthTest::DepthTestFunction         &winding );
unsigned int glWrap( const Blending::Factor                     &factor );
unsigned int glWrap( const Blending::Equation                   &equation );
unsigned int glWrap( const ClearBuffers                         &buffers );
unsigned int glWrap( const RenderState::RasterizationMode       &mode );

/************************************************************************************************/
/*                                            Primitive                                         */
/************************************************************************************************/
unsigned int glWrap( const Primitive &primitive );

}
}

#endif