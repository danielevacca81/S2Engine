// BuiltIn.cpp
//
#include "BuiltIn.h"

#include "OpenGL.h"
#include "utils/String.h"

#include <iostream>

using namespace s2::Renderer;

// ------------------------------------------------------------------------------------------------
s2::Renderer::ProgramPtr BuiltIn::shaderFullscreenQuad;
s2::Renderer::ProgramPtr BuiltIn::shaderSimple;
bool                   BuiltIn::_initialized = false;

s2::Renderer::SamplerPtr BuiltIn::samplerNearestClamp;
s2::Renderer::SamplerPtr BuiltIn::samplerLinearClamp;
s2::Renderer::SamplerPtr BuiltIn::samplerNearestRepeat;
s2::Renderer::SamplerPtr BuiltIn::samplerLinearRepeat;

// ------------------------------------------------------------------------------------------------
static void openglCallbackFunction(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam )
{
	(void) source; (void) type; (void) id;
	(void) severity; (void) length; (void) userParam;
	std::cout << message << std::endl;
	//fprintf( stderr, "%s\n", message );
	//if( severity == GL_DEBUG_SEVERITY_HIGH )
	//{
	//	std::cout << "Aborting: " << message << std::endl;

	//	//fprintf( stderr, "Aborting...\n" );
	//	abort();
	//}
}

// ------------------------------------------------------------------------------------------------
void BuiltIn::enableDebugOutput()
{
	glEnable( GL_DEBUG_OUTPUT );
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
	glDebugMessageCallback( openglCallbackFunction, nullptr );
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true );
}

// ------------------------------------------------------------------------------------------------
bool BuiltIn::init()
{
	if( _initialized )
		return true;

	_initialized = true;
	_initialized &= initShaders();
	_initialized &= initSamplers();

	// @todo: log if failed
	return _initialized;
}

// ------------------------------------------------------------------------------------------------
bool BuiltIn::initShaders()
{
	// @todo: read shaders from resources
	shaderFullscreenQuad = Program::makeNew();

	bool ok = true;
	ok &= shaderFullscreenQuad->attachVertexShader( STRINGIFY(
	#version 400\n
	layout( location = 0 ) in vec3 in_Vertex;
	layout( location = 3 ) in vec2 in_TexCoord;

	out vec2 texCoord;

	void main()
	{
		gl_Position = vec4( in_Vertex, 1.0 );
		texCoord    = in_TexCoord;
	}
	));

	ok &= shaderFullscreenQuad->attachFragmentShader( STRINGIFY(
	#version 400\n

	out vec4 color;
	in vec2 texCoord;

	uniform sampler2D text;

	void main()
	{
		color = texture( text, texCoord );
	}
	) );

	ok &= shaderFullscreenQuad->link( "BuiltIn_shaderFullscreenQuad" );
	return ok;
}

// ------------------------------------------------------------------------------------------------
bool BuiltIn::initSamplers()
{
	samplerNearestClamp  = Sampler::makeNew( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Clamp, Sampler::Wrap::Clamp,  1 );
	samplerLinearClamp   = Sampler::makeNew( Sampler::MinificationFilter::Linear,  Sampler::MagnificationFilter::Linear,  Sampler::Wrap::Clamp, Sampler::Wrap::Clamp,  1 );
	samplerNearestRepeat = Sampler::makeNew( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Repeat,Sampler::Wrap::Repeat, 1 );
	samplerLinearRepeat  = Sampler::makeNew( Sampler::MinificationFilter::Linear,  Sampler::MagnificationFilter::Linear,  Sampler::Wrap::Repeat,Sampler::Wrap::Repeat, 1 );
	return true;
}