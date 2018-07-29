// Surface.cpp
//
#include "Surface.h"

#include "Renderer.h"
#include "OpenGL.h"
#include "utils/String.h"

#include <iostream>

using namespace s2::OpenGL;

static ProgramPtr blit;
void initBlitShaderProgram()
{
	if (blit)
		return;

	blit = Program::New();
	blit->attachVertexShader( STRINGIFY(
	#version 330\n
	layout(location = 0) in vec3 in_Vertex;
	layout(location = 3) in vec2 in_TexCoord;

	out vec2 texCoord;

	void main()
	{
		gl_Position = vec4(in_Vertex, 1.0);
		texCoord = in_TexCoord;
	}
	));

	blit->attachFragmentShader(STRINGIFY(
	#version 330\n

	out vec4 color;
	in vec2 texCoord;

	uniform sampler2D text;

	void main()
	{
		color = texture(text, texCoord);
	}
	));

	blit->link("Fullscreen quad");

	std::cout << blit->info(true) << std::endl;
}


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
	_fbo->attach(FrameBuffer::ColorAttachment0, Texture2D::New(Texture2DDescription(w, h,TextureFormat::RedGreenBlueAlpha8)));
	_fbo->attach(FrameBuffer::DepthAttachment, Texture2D::New(Texture2DDescription(w, h, TextureFormat::Depth24)));

	initBlitShaderProgram();
}

// ------------------------------------------------------------------------------------------------
void Surface::clear(const ClearState &cs)
{
	_fbo->bind();
	_fbo->set();
	_state.setClearState(cs);
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Surface::draw(const Primitive &primitive, const VertexArray &va, const DrawingState &ds)
{
	_fbo->bind();
	_fbo->set();
	_state.setDrawState(ds);

	Renderer::draw(primitive, va);
	//_fbo->unbind();
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Surface::draw(const Primitive &primitive, const Mesh &mesh, const DrawingState &ds)
{
	_fbo->bind();
	_fbo->set();
	_state.setDrawState(ds);

	Renderer::draw(primitive, mesh);
	//_fbo->unbind();
	glCheck;
}

// ------------------------------------------------------------------------------------------------
void Surface::resize(int width, int height)
{
	_fbo->attach(FrameBuffer::ColorAttachment0, Texture2D::New(Texture2DDescription(width,height, TextureFormat::RedGreenBlueAlpha8)));
	_fbo->attach(FrameBuffer::DepthAttachment, Texture2D::New(Texture2DDescription(width, height, TextureFormat::Depth24)));
}

// ------------------------------------------------------------------------------------------------
void Surface::swap(unsigned int targetFBO, const FrameBuffer::AttachmentPoint &att )
{
	glBindFramebuffer( GL_FRAMEBUFFER, targetFBO );
	//OpenGL::RenderState rs;
	//rs.

	/*VertexArray va;*/
	std::vector<Math::vec3> vertices = { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} };
	std::vector<Math::vec2> texCoords = { {0,1},{0,0},{1,1},{1,0} };
	
	Mesh m;
	m.setVertices(vertices);
	m.setTextureCoords(texCoords);

	OpenGL::DrawingState ds(blit);
	_state.setDrawState(ds);
	//glViewport(0, 0, , 512);

	_fbo->attachment(att)->bind();


	Renderer::draw( Primitive::TriangleStrip, m );
	Texture2D::unbind();
}
