// Resources.cpp
//
#include "Resources.h"

#include "Core/Strings.h"

using namespace RenderSystem;

// ------------------------------------------------------------------------------------------------
Resources::Shaders  Resources::DefaultShaders;
Resources::Samplers Resources::DefaultSamplers;
bool                       Resources::_initialized { false };

// ------------------------------------------------------------------------------------------------
bool Resources::init()
{
	if( _initialized )
		return true;

	bool ok = true;
	ok &= initShaders();
	ok &= initSamplers();

	_initialized = ok;
	return ok;
}

// ------------------------------------------------------------------------------------------------
void Resources::destroy()
{
	if( !_initialized )
		return;

	DefaultShaders.Simple         = nullptr;
	DefaultShaders.FullscreenQuad = nullptr;

	DefaultSamplers.NearestClamp  = nullptr;
	DefaultSamplers.LinearClamp   = nullptr;
	DefaultSamplers.NearestRepeat = nullptr;
	DefaultSamplers.LinearRepeat  = nullptr;
}

// ------------------------------------------------------------------------------------------------
bool Resources::initSamplers()
{
	DefaultSamplers.NearestClamp  = Sampler::New( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Clamp, Sampler::Wrap::Clamp, 1 );
	DefaultSamplers.LinearClamp   = Sampler::New( Sampler::MinificationFilter::Linear , Sampler::MagnificationFilter::Linear , Sampler::Wrap::Clamp, Sampler::Wrap::Clamp, 1 );
	DefaultSamplers.NearestRepeat = Sampler::New( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Repeat, Sampler::Wrap::Repeat, 1 );
	DefaultSamplers.LinearRepeat  = Sampler::New( Sampler::MinificationFilter::Linear , Sampler::MagnificationFilter::Linear , Sampler::Wrap::Repeat, Sampler::Wrap::Repeat, 1 );
	return true;
}


// ------------------------------------------------------------------------------------------------
bool Resources::initShaders()
{
	// @todo: read shaders from resources
	bool ok = true;
	DefaultShaders.FullscreenQuad = Program::New();
	{
		ok &= DefaultShaders.FullscreenQuad->attachVertexShader( STRINGIFY( #version 330\n
		layout( location = 0 ) in vec3 in_Vertex;
		layout( location = 3 ) in vec2 in_TexCoord;

		out vec2 texCoord;

		void main()
		{
			gl_Position = vec4( in_Vertex, 1.0 );
			texCoord = in_TexCoord;
		}
		) );

		ok &= DefaultShaders.FullscreenQuad->attachFragmentShader( STRINGIFY( #version 330\n

		out vec4 color;
		in vec2 texCoord;

		uniform sampler2D text;

		void main()
		{
			color = texture( text, texCoord );
		}
		) );

		ok &= DefaultShaders.FullscreenQuad->link( "DefaultShaders.FullscreenQuad" );
	}

	// -----------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------
	DefaultShaders.Simple = Program::New();
	{
		ok &= DefaultShaders.Simple->attachVertexShader( STRINGIFY( #version 330\n
		layout( location = 0 ) in vec3 in_Vertex;
		layout( location = 1 ) in vec4 in_Color;

		uniform mat4 modelViewProjectionMatrix;

		out vec4 color;

		void main()
		{
			gl_Position = modelViewProjectionMatrix * vec4( in_Vertex, 1.0 );
			color = in_Color;
			//color       = vec4( 1.0, 0.0, 1.0, 1.0 );
		}
		) );

		ok &= DefaultShaders.Simple->attachFragmentShader( STRINGIFY( #version 330\n

													in vec4 color;
		out vec4 fragColor;

		void main()
		{
			fragColor = color;
		}
		) );

		ok &= DefaultShaders.Simple->link( "DefaultShaders.Simple" );
	}
	return ok;
}