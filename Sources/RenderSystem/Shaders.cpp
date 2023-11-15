// Shaders.cpp
//
#include "Shaders.h"

#include "Core/Strings.h"

using namespace RenderSystem;

// ------------------------------------------------------------------------------------------------
ProgramPtr Shaders::FullscreenQuad;
ProgramPtr Shaders::Simple;
bool       Shaders::_initialized { false };


// ------------------------------------------------------------------------------------------------
bool Shaders::init()
{
	if( _initialized )
		return true;

	// @todo: read shaders from resources
	bool ok = true;
	FullscreenQuad = Program::New();
	{
		ok &= FullscreenQuad->attachVertexShader( STRINGIFY( #version 330\n
		layout( location = 0 ) in vec3 in_Vertex;
		layout( location = 3 ) in vec2 in_TexCoord;

		out vec2 texCoord;

		void main()
		{
			gl_Position = vec4( in_Vertex, 1.0 );
			texCoord = in_TexCoord;
		}
		) );

		ok &= FullscreenQuad->attachFragmentShader( STRINGIFY( #version 330\n

													out vec4 color;
		in vec2 texCoord;

		uniform sampler2D text;

		void main()
		{
			color = texture( text, texCoord );
		}
		) );

		ok &= FullscreenQuad->link( "BuiltIn_shaderFullscreenQuad" );
	}

	// -----------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------
	Simple = Program::New();
	{
		ok &= Simple->attachVertexShader( STRINGIFY( #version 330\n
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

		ok &= Simple->attachFragmentShader( STRINGIFY( #version 330\n

											in vec4 color;
		out vec4 fragColor;

		void main()
		{
			fragColor = color;
		}
		) );

		ok &= Simple->link( "SimpleShader" );
	}


	_initialized = ok;
	return ok;
}

// ------------------------------------------------------------------------------------------------
void Shaders::destroy()
{
	if( !_initialized )
		return;

	Simple = nullptr;
	FullscreenQuad = nullptr;
}