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
unsigned int glWrap( IndexBuffer::IndexDataType datatype );

/************************************************************************************************/
/*                                         AttributeBuffer                                      */
/************************************************************************************************/
unsigned int glWrap( AttributeBuffer::ComponentDatatype datatype );

/************************************************************************************************/
/*                                        VertexBufferObject                                    */
/************************************************************************************************/
unsigned int glWrap( BufferObject::BufferUsageHint hint);
unsigned int glWrap( BufferObject::BufferType type );
unsigned int glWrap( BufferObject::BufferMapMode mode );

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
unsigned int glWrap( Texture::TextureType      type );
unsigned int glWrap( Texture::DataFormat       format );
unsigned int glWrap( Texture::DataType         type );
unsigned int glWrap( Texture::TextureMinFilter filter );
unsigned int glWrap( Texture::TextureMagFilter filter );
unsigned int glWrap( Texture::WrapMode         type );

/************************************************************************************************/
/*                                           FrameBuffer                                        */
/************************************************************************************************/
unsigned int glWrap( FrameBuffer::AttachmentPoint attachment );
unsigned int glWrap( FrameBuffer::DepthFormat     format );

/************************************************************************************************/
/*                                           RenderState                                        */
/************************************************************************************************/
unsigned int glWrap( FaceCulling::Face                    face );
unsigned int glWrap( FaceCulling::WindingOrder            winding );
unsigned int glWrap( StencilTestFace::StencilOperation    op );
unsigned int glWrap( StencilTestFace::StencilTestFunction fun );
unsigned int glWrap( DepthTest::DepthTestFunction         winding );
unsigned int glWrap( Blending::BlendingFactor             factor );
unsigned int glWrap( Blending::BlendEquation              equation );
unsigned int glWrap( ClearBuffers                         buffers );
unsigned int glWrap( RenderState::RasterizationMode       mode);

/************************************************************************************************/
/*                                            Primitive                                         */
/************************************************************************************************/
unsigned int glWrap( PrimitiveType primitive );

}

#endif