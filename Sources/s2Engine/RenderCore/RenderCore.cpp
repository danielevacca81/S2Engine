// RenderCore.cpp
//
#include "RenderCore.h"

#include "Core/Strings.h"

#include "ShaderStage.h"
#include "ShaderCompiler.h"

namespace s2 {
namespace RenderCore {
static bool gInitialized { false }; // Flag to check if the resources have been initialized. @todo: once per context

Samplers DefaultSamplers; // Default samplers
Shaders  DefaultShaders; // Default shaders

// ------------------------------------------------------------------------------------------------
static inline bool initSamplers()
{
	DefaultSamplers.NearestClamp  = Sampler::New( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Clamp, Sampler::Wrap::Clamp, 1 );
	DefaultSamplers.LinearClamp   = Sampler::New( Sampler::MinificationFilter::Linear , Sampler::MagnificationFilter::Linear , Sampler::Wrap::Clamp, Sampler::Wrap::Clamp, 1 );
	DefaultSamplers.NearestRepeat = Sampler::New( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Repeat, Sampler::Wrap::Repeat, 1 );
	DefaultSamplers.LinearRepeat  = Sampler::New( Sampler::MinificationFilter::Linear , Sampler::MagnificationFilter::Linear , Sampler::Wrap::Repeat, Sampler::Wrap::Repeat, 1 );
	return true;
}

// ------------------------------------------------------------------------------------------------
static inline bool initShaders()
{
	// @todo: read shaders from resources
	bool ok = true;
	// #####################################
	DefaultShaders.FullscreenQuad = Program::New();
	{
		auto vtxOk = ShaderCompiler::compile( ShaderStageType::Vertex, STRINGIFY( #version 330\n
        const vec3 in_Vertex[4] = vec3[4]( 
                       vec3( -1.0,  1.0, 0.0 ), 
				       vec3( -1.0, -1.0, 0.0 ),
				       vec3(  1.0,  1.0, 0.0 ),
					   vec3(  1.0, -1.0, 0.0 ) );

		out vec2 texCoord;

		void main()
		{
			vec4 pos4   = vec4( in_Vertex[gl_VertexID], 1.0 );
			gl_Position = pos4;
			texCoord    = vec2( ( pos4.x + 1.0 ) / 2.0, ( pos4.y + 1.0 ) / 2.0 );
		}) );

		auto frgOk = ShaderCompiler::compile( ShaderStageType::Fragment, STRINGIFY( #version 330\n
		out vec4 color;
		in vec2 texCoord;

		uniform sampler2D text;

		void main()
		{
			color = texture( text, texCoord );
		}) );

		if( vtxOk && frgOk )
		{
			ok &= DefaultShaders.FullscreenQuad->attachVertexShader( vtxOk.stage );
			ok &= DefaultShaders.FullscreenQuad->attachFragmentShader( frgOk.stage );
			ok &= ShaderCompiler::linkProgram( DefaultShaders.FullscreenQuad, "DefaultShaders.FullscreenQuad" ).success;
		}
	}

	// #####################################
	DefaultShaders.Simple = Program::New();
	{
		auto vtxOk = ShaderCompiler::compile( ShaderStageType::Vertex, STRINGIFY( #version 330\n
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

		auto frgOk = ShaderCompiler::compile( ShaderStageType::Fragment, STRINGIFY( #version 330\n
        in vec4 color;
		out vec4 fragColor;

		void main()
		{
			fragColor = color;
		}
		) );

		if( vtxOk && frgOk )
		{
			ok &= DefaultShaders.Simple->attachVertexShader( vtxOk.stage );
			ok &= DefaultShaders.Simple->attachFragmentShader( frgOk.stage );
			ok &= ShaderCompiler::linkProgram( DefaultShaders.Simple, "DefaultShaders.Simple" ).success;
		}
	}

	// #####################################
	DefaultShaders.Phong = Program::New();
	{
		auto vtxOk = ShaderCompiler::compile( ShaderStageType::Vertex, STRINGIFY( #version 330\n
        layout( location = 0 ) in vec3 in_Vertex;
		layout( location = 1 ) in vec4 in_Color;
		layout( location = 2 ) in vec3 in_Normal;

		uniform mat4 modelViewProjectionMatrix;
		uniform mat3 normalMatrix;

		out vec3 position;
		out vec3 normal;
		out vec4 color;

		void main()
		{
			gl_Position = modelViewProjectionMatrix * vec4( in_Vertex, 1.0 );
			
			position = gl_Position.xyz;
			normal   = normalize( normalMatrix * in_Normal );
			color    = in_Color;
		}
		) );

		auto frgOk = ShaderCompiler::compile( ShaderStageType::Fragment, STRINGIFY( #version 330\n
		uniform vec4  u_LightPosition;
		uniform vec4  u_LightAmbient;
		uniform vec4  u_LightDiffuse;
		uniform vec4  u_LightSpecular;
		uniform float u_LightShininess;
														   
	    in vec3 position;
		in vec3 normal;
		in vec4 color;

		out vec4 fragColor;

		void main()
		{
			vec3 E = normalize( -position ); // we are in Eye Coordinates, so EyePos is (0,0,0)  
			vec3 L = normalize( u_LightPosition.xyz - position );
			vec3 R = normalize( -reflect( L, normal ) );

			//calculate Ambient Term:  
			vec4 Iamb = u_LightAmbient;

			//calculate Diffuse Term:  
			vec4 Idiff = u_LightDiffuse * max( dot( normal, L ), 0.0 );
			Idiff = clamp( Idiff, 0.0, 1.0 );

			// calculate Specular Term:
			vec4 Ispec = u_LightSpecular * pow( max( dot( R, E ), 0.0 ), u_LightShininess );
			Ispec = clamp( Ispec, 0.0, 1.0 );

			// write Total Color:  
			fragColor = color * ( Iamb + Idiff + Ispec );
		}
		) );

		if( vtxOk && frgOk )
		{
			ok &= DefaultShaders.Phong->attachVertexShader( vtxOk.stage );
			ok &= DefaultShaders.Phong->attachFragmentShader( frgOk.stage );
			ok &= ShaderCompiler::linkProgram( DefaultShaders.Phong, "DefaultShaders.Phong" ).success;
		}
	}

	// #####################################
	DefaultShaders.BlinnPhong = Program::New();
	{
		auto vtxOk = ShaderCompiler::compile( ShaderStageType::Vertex, STRINGIFY( #version 330\n
        layout( location = 0 ) in vec3 in_Vertex;
		layout( location = 1 ) in vec4 in_Color;
		layout( location = 2 ) in vec3 in_Normal;

		uniform mat4 modelViewProjectionMatrix;
		uniform mat4 modelViewMatrix;
		uniform mat3 normalMatrix;

		out vec3 position;
		out vec3 normal;
		out vec4 color;

		void main()
		{			
			vec4 vertPos4 = modelViewMatrix * vec4( in_Vertex, 1.0 );

			position      = vec3( vertPos4 ) / vertPos4.w;
			normal        = normalize( normalMatrix * in_Normal );
			color         = in_Color;
		
			gl_Position = modelViewProjectionMatrix * vec4( in_Vertex, 1.0 );
		}
		) );

		auto frgOk = ShaderCompiler::compile( ShaderStageType::Fragment, STRINGIFY( #version 330\n
		uniform vec4  u_LightPosition;
		uniform vec4  u_LightAmbient;
		uniform vec4  u_LightDiffuse;
		uniform vec4  u_LightSpecular;
		uniform float u_LightShininess;

	    in vec3 position;
		in vec3 normal;
		in vec4 color;

		out vec4 fragColor;

		void main()
		{
			vec3 N = normalize( normal );
			vec3 L = normalize( u_LightPosition.xyz - position );
			float lambertian = max( dot( L, N ), 0.0 );
			float specular = 0.0;

			vec3 D = normalize( -position );

			vec3 halfDir = normalize( L + D );
			float specAngle = max( dot( halfDir, N ), 0.0 );
			specular = pow( specAngle, u_LightShininess );

			fragColor = color *
				vec4( u_LightAmbient +
					  u_LightDiffuse * lambertian +
					  u_LightSpecular * specular );
		}
		) );

		if( vtxOk && frgOk )
		{
			ok &= DefaultShaders.BlinnPhong->attachVertexShader( vtxOk.stage );
			ok &= DefaultShaders.BlinnPhong->attachFragmentShader( frgOk.stage );
			ok &= ShaderCompiler::linkProgram( DefaultShaders.BlinnPhong, "DefaultShaders.BlinnPhong" ).success;
		}
	}
	return ok;
}



// ------------------------------------------------------------------------------------------------
bool init()
{
	if( gInitialized )
		return true;

	bool ok = true;
	ok &= initShaders();
	ok &= initSamplers();

	gInitialized = ok;
	return ok;
}

// ------------------------------------------------------------------------------------------------
void destroy()
{
	if( !gInitialized )
		return;

	DefaultShaders.Simple         = nullptr;
	DefaultShaders.Phong          = nullptr;
	DefaultShaders.BlinnPhong     = nullptr;
	DefaultShaders.FullscreenQuad = nullptr;

	DefaultSamplers.NearestClamp  = nullptr;
	DefaultSamplers.LinearClamp   = nullptr;
	DefaultSamplers.NearestRepeat = nullptr;
	DefaultSamplers.LinearRepeat  = nullptr;
}

}
}