// Surface.cpp
//
#include "Surface.h"

#include "Renderer.h"
#include "OpenGL.h"
#include "Texture.h"
#include "TextureUnit.h"
#include "Sampler.h"
#include "BuiltIn.h"


#include <iostream>

using namespace s2::OpenGL;

// @todo: link to context?

// ------------------------------------------------------------------------------------------------
SurfacePtr Surface::New()
{
	return std::make_shared<Surface>();
}

// ------------------------------------------------------------------------------------------------
Surface::Surface()
{
	init();//  ?
}

// ------------------------------------------------------------------------------------------------
Surface::~Surface()
{}

// ------------------------------------------------------------------------------------------------
void Surface::init()
{
	const int w = 64;
	const int h = 64;

	_fbo.create();
	_fbo.attach( FrameBuffer::ColorAttachment0, std::make_shared<Texture2D>( TextureDescription( w, h, TextureFormat::RedGreenBlueAlpha8 ) ) );
	_fbo.attach( FrameBuffer::DepthAttachment,  std::make_shared<Texture2D>( TextureDescription( w, h, TextureFormat::Depth24 ) ) );
}

// ------------------------------------------------------------------------------------------------
void Surface::clear( const ClearState &cs )
{
	_fbo.bind();
	_state.setClearState( cs );
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Surface::draw( const Primitive &primitive, const VertexArray &va, const DrawingState &ds )
{
	_fbo.bind();
	_state.setDrawState( ds );

	Renderer::draw( primitive, va );
}

// ------------------------------------------------------------------------------------------------
void Surface::draw( const Primitive &primitive, const Mesh &mesh, const DrawingState &ds )
{
	_fbo.bind();
	_state.setDrawState( ds );

	Renderer::draw( primitive, mesh );
}

// ------------------------------------------------------------------------------------------------
void Surface::resize( int width, int height )
{
	_fbo.attach( FrameBuffer::ColorAttachment0, std::make_shared<Texture2D>( TextureDescription( width, height, TextureFormat::RedGreenBlueAlpha8 ) ) );
	_fbo.attach( FrameBuffer::DepthAttachment,  std::make_shared<Texture2D>( TextureDescription( width, height, TextureFormat::Depth24 ) ) );
}

// ------------------------------------------------------------------------------------------------
void Surface::swap( unsigned int targetFBO, const FrameBuffer::AttachmentPoint &att )
{
	glBindFramebuffer( GL_FRAMEBUFFER, targetFBO );

	DrawingState ds( BuiltIn::shaderFullscreenQuad.shared_from_this() );
	ds.textureUnits[ 0 ].setTexture( _fbo.attachment( att ) );
	ds.textureUnits[ 0 ].setSampler( BuiltIn::samplerLinearClamp.shared_from_this() );

	_state.setDrawState( ds );

	Mesh       quad;
	std::vector<Math::vec3> vertices  = { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} };
	std::vector<Math::vec2> texCoords = { {0,1},{0,0},{1,1},{1,0} };

	quad.setVertices( vertices );
	quad.setTextureCoords( texCoords );

	Renderer::draw( Primitive::TriangleStrip, quad );
}
