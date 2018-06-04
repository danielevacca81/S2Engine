// OpenGLWrap.h
// 
#ifndef OPENGLWRAP_ONCE
#define OPENGLWRAP_ONCE

#include "OpenGL.h"

#include "Texture.h"
#include "FrameBuffer.h"
#include "VertexBuffer.h"
#include "AttributeBuffer.h"
#include "IndexBuffer.h"

#include "Renderer/RenderState.h"

#define BUFFER_OFFSET(i) ((void*)(i))

namespace OpenGL
{
/************************************************************************************************/
/*                                          IndexBuffer                                         */
/************************************************************************************************/
unsigned int glWrap( const IndexBuffer::IndexDataType &datatype );

/************************************************************************************************/
/*                                         AttributeBuffer                                      */
/************************************************************************************************/
unsigned int glWrap( const AttributeBuffer::ComponentDatatype &datatype );

/************************************************************************************************/
/*                                        VertexBufferObject                                    */
/************************************************************************************************/
unsigned int glWrap( const BufferObject::BufferUsageHint &hint);
unsigned int glWrap( const BufferObject::BufferType      &type );
unsigned int glWrap( const BufferObject::BufferMapMode   &mode );

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
unsigned int glWrap( const Texture::TextureType      &type );
unsigned int glWrap( const Texture::DataFormat       &format );
unsigned int glWrap( const Texture::DataType         &type );
unsigned int glWrap( const Texture::TextureMinFilter &filter );
unsigned int glWrap( const Texture::TextureMagFilter &filter );
unsigned int glWrap( const Texture::WrapMode         &type );

/************************************************************************************************/
/*                                           FrameBuffer                                        */
/************************************************************************************************/
unsigned int glWrap( const FrameBuffer::AttachmentPoint &attachment );
unsigned int glWrap( const FrameBuffer::DepthFormat     &format );

/************************************************************************************************/
/*                                           RenderState                                        */
/************************************************************************************************/
unsigned int glWrap( const FaceCulling::Face                    &face );
unsigned int glWrap( const FaceCulling::WindingOrder            &winding );
unsigned int glWrap( const StencilTestFace::StencilOperation    &op );
unsigned int glWrap( const StencilTestFace::StencilTestFunction &fun );
unsigned int glWrap( const DepthTest::DepthTestFunction         &winding );
unsigned int glWrap( const Blending::BlendingFactor             &factor );
unsigned int glWrap( const Blending::BlendEquation              &equation );
unsigned int glWrap( const ClearBuffers                         &buffers );
unsigned int glWrap( const RenderState::RasterizationMode       &mode);

/************************************************************************************************/
/*                                            Primitive                                         */
/************************************************************************************************/
unsigned int glWrap( const PrimitiveType &primitive );

}

#endif