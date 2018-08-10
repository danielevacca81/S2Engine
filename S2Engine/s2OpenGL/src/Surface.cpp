// Surface.cpp
//
#include "Surface.h"

#include "Renderer.h"
#include "OpenGL.h"
#include "Texture.h"
#include "TextureUnit.h"
#include "TextureSampler.h"

#include "utils/String.h"

#include <iostream>

using namespace s2::OpenGL;

/************************************************************************************************/
/*                                           QuadShader                                         */
/************************************************************************************************/
#pragma region QuadShader
static ProgramPtr quadShader;
static void initBlitShaderProgram()
{
	if (quadShader)
		return;

	quadShader = Program::New();
	quadShader->attachVertexShader( STRINGIFY(
		#version 400\n
		layout(location = 0) in vec3 in_Vertex;
		layout(location = 3) in vec2 in_TexCoord;

		out vec2 texCoord;

		void main()
		{
			gl_Position = vec4(in_Vertex, 1.0);
			texCoord = in_TexCoord;
		}
	));

	quadShader->attachFragmentShader(STRINGIFY(
		#version 400\n

		out vec4 color;
		in vec2 texCoord;

		uniform sampler2D text;

		void main()
		{
			color = texture(text, texCoord);
		}
	));

	quadShader->link("QuadShader");
}
#pragma endregion

// ------------------------------------------------------------------------------------------------
SurfacePtr Surface::New()
{
	return std::make_shared<Surface>();
}

// ------------------------------------------------------------------------------------------------
Surface::Surface()
{
	int w = 64;
	int h = 64;


	_fbo = FrameBuffer::New();
	_fbo->attach(FrameBuffer::ColorAttachment0, Texture2D::New(TextureDescription(w, h,TextureFormat::RedGreenBlueAlpha8)));
	_fbo->attach(FrameBuffer::DepthAttachment, Texture2D::New(TextureDescription(w, h, TextureFormat::Depth24)));

	initBlitShaderProgram();
}

// ------------------------------------------------------------------------------------------------
void Surface::clear(const ClearState &cs)
{
	_fbo->bind();
	_state.setClearState(cs);
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Surface::draw(const Primitive &primitive, const VertexArray &va, const DrawingState &ds)
{
	_fbo->bind();
	_state.setDrawState(ds);

	Renderer::draw(primitive, va);
}

// ------------------------------------------------------------------------------------------------
void Surface::draw(const Primitive &primitive, const Mesh &mesh, const DrawingState &ds)
{
	_fbo->bind();
	_state.setDrawState(ds);

	Renderer::draw(primitive, mesh);
}

// ------------------------------------------------------------------------------------------------
void Surface::resize(int width, int height)
{
	_fbo->attach(FrameBuffer::ColorAttachment0, Texture2D::New(TextureDescription(width,height, TextureFormat::RedGreenBlueAlpha8)));
	_fbo->attach(FrameBuffer::DepthAttachment, Texture2D::New(TextureDescription(width, height, TextureFormat::Depth24)));
}

// ------------------------------------------------------------------------------------------------
void Surface::swap(unsigned int targetFBO, const FrameBuffer::AttachmentPoint &att )
{
	glBindFramebuffer( GL_FRAMEBUFFER, targetFBO );

	DrawingState ds(quadShader);
	ds.textureUnits.unit( 0 ).setTexture( _fbo->attachment(att) );
	ds.textureUnits.unit( 0 ).setTextureSampler( TextureSampler::linearClamp() );

	_state.setDrawState(ds);

	Mesh       quad;
	std::vector<s2::Math::vec3> vertices = { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} };
	std::vector<s2::Math::vec2> texCoords = { {0,1},{0,0},{1,1},{1,0} };
	
	quad.setVertices(vertices);
	quad.setTextureCoords(texCoords);

	Renderer::draw( Primitive::TriangleStrip, quad );
}
