// BuiltIn.cpp
//
#include "BuiltIn.h"

#include "OpenGL.h"
#include "Core/StringTools.h"

#include <iostream>

using namespace Renderer;

// ------------------------------------------------------------------------------------------------
Renderer::ProgramPtr BuiltIn::shaderFullscreenQuad;
Renderer::ProgramPtr BuiltIn::shaderSimple;
bool                 BuiltIn::_initialized  = false;
bool                 BuiltIn::_debugEnabled = false;

Renderer::SamplerPtr BuiltIn::samplerNearestClamp;
Renderer::SamplerPtr BuiltIn::samplerLinearClamp;
Renderer::SamplerPtr BuiltIn::samplerNearestRepeat;
Renderer::SamplerPtr BuiltIn::samplerLinearRepeat;

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

	if( severity == GL_DEBUG_SEVERITY_HIGH
	||  severity == GL_DEBUG_SEVERITY_MEDIUM )
		//|| severity == GL_DEBUG_SEVERITY_LOW )
		std::cout << severity << " " << message << '\n';
	
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
	glCheck;

	_debugEnabled = true;
}


// ------------------------------------------------------------------------------------------------
void BuiltIn::disableDebugOutput()
{
	glDisable( GL_DEBUG_OUTPUT );
	glDisable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
	glDebugMessageCallback( nullptr, nullptr );
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, false );
	glCheck;

	_debugEnabled = false;
}

// ------------------------------------------------------------------------------------------------
bool BuiltIn::isDebugOutputEnabled() { return _debugEnabled; }


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
	bool ok = true;
	shaderFullscreenQuad = Program::New();
	{
		ok &= shaderFullscreenQuad->attachVertexShader( STRINGIFY(#version 330\n
			layout( location = 0 ) in vec3 in_Vertex;
		layout( location = 3 ) in vec2 in_TexCoord;

		out vec2 texCoord;

		void main()
		{
			gl_Position = vec4( in_Vertex, 1.0 );
			texCoord    = in_TexCoord;
		}
		) );

		ok &= shaderFullscreenQuad->attachFragmentShader( STRINGIFY(#version 330\n

			out vec4 color;
		in vec2 texCoord;

		uniform sampler2D text;

		void main()
		{
			color = texture( text, texCoord );
		}
		) );

		ok &= shaderFullscreenQuad->link( "BuiltIn_shaderFullscreenQuad" );
	}

	// -----------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------
	shaderSimple = Program::New();
	{
		ok &= shaderSimple->attachVertexShader( STRINGIFY(#version 330\n

		layout( location = 0 ) in vec3 in_Vertex;
		layout( location = 1 ) in vec4 in_Color;

		uniform mat4 modelViewProjectionMatrix;
		
		out vec4 color;

		void main()
		{
			gl_Position = modelViewProjectionMatrix * vec4( in_Vertex, 1.0 );
			color       = in_Color;
			//color       = vec4( 1.0, 0.0, 1.0, 1.0 );
		}
		) );

		ok &= shaderSimple->attachFragmentShader( STRINGIFY(#version 330\n

		in vec4 color;
		out vec4 fragColor;

		void main()
		{
			fragColor = color;
		}
		) );

		ok &= shaderSimple->link( "SimpleShader" );
	}


	return ok;
}

// ------------------------------------------------------------------------------------------------
bool BuiltIn::initSamplers()
{
	samplerNearestClamp  = Sampler::New( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Clamp, Sampler::Wrap::Clamp,  1 );
	samplerLinearClamp   = Sampler::New( Sampler::MinificationFilter::Linear,  Sampler::MagnificationFilter::Linear,  Sampler::Wrap::Clamp, Sampler::Wrap::Clamp,  1 );
	samplerNearestRepeat = Sampler::New( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Repeat,Sampler::Wrap::Repeat, 1 );
	samplerLinearRepeat  = Sampler::New( Sampler::MinificationFilter::Linear,  Sampler::MagnificationFilter::Linear,  Sampler::Wrap::Repeat,Sampler::Wrap::Repeat, 1 );
	return true;
}