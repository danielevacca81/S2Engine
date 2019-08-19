// Surface.cpp
//
#include "Surface.h"

#include "Renderer.h"
#include "OpenGL.h"
#include "OpenGLWrap.h"
#include "Texture.h"
#include "TextureUnit.h"
#include "Sampler.h"
#include "BuiltIn.h"



#include <iostream>

using namespace s2::Renderer;

// @todo: link to context?

// ------------------------------------------------------------------------------------------------
SurfacePtr Surface::New()
{
	return std::make_shared<Surface>();
}

// ------------------------------------------------------------------------------------------------
Surface::Surface()
: _width( 0 )
, _height( 0 )
{
	create();
}

// ------------------------------------------------------------------------------------------------
Surface::~Surface()
{}

// ------------------------------------------------------------------------------------------------
void Surface::create()
{
	_fbo = FrameBuffer::New();
	resize( 64, 64 );
}

// ------------------------------------------------------------------------------------------------
void Surface::clear( const ClearState &cs )
{
	_fbo->bind();
	_state.setClearState( cs );
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Surface::draw( const Primitive &primitive, const VertexArrayPtr &va, const DrawingState &ds )
{
	_fbo->bind();
	_state.setDrawState( ds );

	RenderingHelper::draw( primitive, va );
}

// ------------------------------------------------------------------------------------------------
void Surface::draw( const Primitive &primitive, const PrimitiveBufferPtr &mesh, const DrawingState &ds )
{
	_fbo->bind();
	_state.setDrawState( ds );

	RenderingHelper::draw( primitive, mesh );
}

// ------------------------------------------------------------------------------------------------
void Surface::resize( int width, int height )
{
	_width  = width;
	_height = height;

	_fbo->attach( FrameBuffer::ColorAttachment0, Texture2D::New( TextureDescription( width, height, TextureFormat::RedGreenBlueAlpha8 ) ) );
	_fbo->attach( FrameBuffer::DepthAttachment,  Texture2D::New( TextureDescription( width, height, TextureFormat::Depth24 ) ) );
}

// ------------------------------------------------------------------------------------------------
void Surface::swap( unsigned int targetFBO, const FrameBuffer::AttachmentPoint &att )
{
	glBindFramebuffer( GL_FRAMEBUFFER, targetFBO );

	DrawingState ds( BuiltIn::shaderFullscreenQuad );
	ds.textureUnits[ 0 ].setTexture( _fbo->attachment( att ) );
	ds.textureUnits[ 0 ].setSampler( BuiltIn::samplerLinearClamp );

	_state.setDrawState( ds );

	PrimitiveBufferPtr      quad = PrimitiveBuffer::New();
	std::vector<Math::vec3> vertices  = { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} };
	std::vector<Math::vec2> texCoords = { {0,1},{0,0},{1,1},{1,0} };

	quad->setVertices( vertices );
	quad->setTextureCoords( texCoords );

	RenderingHelper::draw( Primitive::TriangleStrip, quad );
}

// ------------------------------------------------------------------------------------------------
s2::ImageBufferPtr<unsigned char> Surface::grabImage() const
{
	// @todo: ok for multisample buffers?
	
	//auto img = _fbo->attachment( FrameBuffer::ColorAttachment0 )->readData();
	const int rowAlignment       = 4;
	const ImageFormat format     = ImageFormat::RedGreenBlueAlpha;
	const ImageDataType dataType = ImageDataType::UnsignedByte;
	const int sizeInBytes        = computeRequiredSizeInBytes( _width, _height, format, dataType, rowAlignment );

	_fbo->bind();
	//glPixelStorei( GL_PACK_ALIGNMENT, rowAlignment );
	
	ReadPixelBuffer pixelBuffer = ReadPixelBuffer( sizeInBytes, ReadPixelBuffer::UsageHint::Static );
	pixelBuffer.bind();
	
	glReadPixels( 0,0, _width, _height, glWrap( format ), glWrap( dataType ), BUFFER_OFFSET(0) );
	glCheck;
	//unbind();
	//glCheck;

	s2::ImageBufferPtr<unsigned char> img = s2::ImageBuffer<unsigned char>::New( _width, _height, 4, (unsigned char*)pixelBuffer.mapData() );
	pixelBuffer.unmapData();
	pixelBuffer.unbind();
	_fbo->unbind();

	return img;
}
