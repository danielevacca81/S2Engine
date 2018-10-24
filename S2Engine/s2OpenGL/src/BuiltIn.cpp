// BuiltIn.cpp
//
#include "BuiltIn.h"

#include "utils/String.h"

using namespace s2::OpenGL;

// ------------------------------------------------------------------------------------------------
s2::OpenGL::Program BuiltIn::shaderFullscreenQuad;
s2::OpenGL::Program BuiltIn::shaderSimple;
bool                BuiltIn::_initialized = false;


s2::OpenGL::Sampler BuiltIn::samplerNearestClamp;
s2::OpenGL::Sampler BuiltIn::samplerLinearClamp;
s2::OpenGL::Sampler BuiltIn::samplerNearestRepeat;
s2::OpenGL::Sampler BuiltIn::samplerLinearRepeat;

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
	shaderFullscreenQuad.create();

	bool ok = true;
	ok &= shaderFullscreenQuad.attachVertexShader( STRINGIFY(
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

	ok &= shaderFullscreenQuad.attachFragmentShader( STRINGIFY(
	#version 400\n

	out vec4 color;
	in vec2 texCoord;

	uniform sampler2D text;

	void main()
	{
		color = texture( text, texCoord );
	}
	) );

	ok &= shaderFullscreenQuad.link( "BuiltIn_shaderFullscreenQuad" );
	return ok;
}

// ------------------------------------------------------------------------------------------------
bool BuiltIn::initSamplers()
{
	(samplerNearestClamp  = Sampler( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Clamp, Sampler::Wrap::Clamp, 1 )).create();
	(samplerLinearClamp   = Sampler( Sampler::MinificationFilter::Linear,  Sampler::MagnificationFilter::Linear,  Sampler::Wrap::Clamp, Sampler::Wrap::Clamp, 1 )).create();
	(samplerNearestRepeat = Sampler( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Repeat,Sampler::Wrap::Repeat, 1 )).create();
	(samplerLinearRepeat  = Sampler( Sampler::MinificationFilter::Linear,  Sampler::MagnificationFilter::Linear,  Sampler::Wrap::Repeat,Sampler::Wrap::Repeat, 1 )).create();
	return true;
}