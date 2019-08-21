#include "GLResourcesLoader.h"

#include "renderer/BuiltIn.h"
#include "renderer/Extensions.h"
#include "renderer/Program.h"

#include "math/Mesh.h"
#include "math/Geometry.h"
#include "utils/String.h"

#include <iostream>


s2::Renderer::PrimitiveBufferPtr  GLResourcesLoader::_torus;
s2::Renderer::PrimitiveBufferPtr  GLResourcesLoader::_cube;
s2::Renderer::ProgramPtr          GLResourcesLoader::_pipeShader;
s2::Renderer::ProgramPtr          GLResourcesLoader::_phong;
s2::Renderer::ProgramPtr          GLResourcesLoader::_background;
s2::Renderer::ProgramPtr          GLResourcesLoader::_simpleShader;
s2::Renderer::ContextPtr          GLResourcesLoader::_mainContext;
//int                  GLResourcesLoader::_composite = -666;

using namespace s2;

// ------------------------------------------------------------------------------------------------
GLResourcesLoader::GLResourcesLoader( QWidget *parent )
: QOpenGLWidget(parent)
{
	//create();
	//initSharedResources();
	std::cout << "Constructor GLSharedResources" << std::endl;
}


// ------------------------------------------------------------------------------------------------
GLResourcesLoader::~GLResourcesLoader()
{
	//_mainContext->release();
	//_mainContext = nullptr;

	makeCurrent();
	//s2::OpenGL::BuiltIn::destroy();

	_phong      = nullptr;
	_cube       = nullptr;
	_torus      = nullptr;
	_background = nullptr;

	//myVR::destroyComposite(_composite);
	//myVR::destroy(nullptr); 

	//doneCurrent();
}

 //------------------------------------------------------------------------------------------------
void GLResourcesLoader::initializeGL()
{
	hide();
	
#if 1
	_mainContext = Renderer::Context::Current(); // initializes extensions also

	std::cout << "Context ID: " << _mainContext->id() << std::endl 
		<< _mainContext->info() << std::endl;

	s2::Renderer::BuiltIn::enableDebugOutput();
	s2::Renderer::BuiltIn::init();


	initMyVR();

	initSharedResources();

	emit resourcesInitialized();
#endif
}

// ------------------------------------------------------------------------------------------------
bool GLResourcesLoader::initSharedResources()
{
	initMeshes();
	initShaders();
	return true;
}

// ------------------------------------------------------------------------------------------------
bool GLResourcesLoader::initMyVR()
{
 //   myVR::initialize(JSON_mMapInitialize,nullptr);
 //   _composite = myVR::createComposite();

	//std::cout << "Composite ID " << _composite << std::endl;

 //   // set up layers;
 //   std::string jsonOut;
 //   myVR::execute(_composite, JSON_Layer_OpenStreetMapService,jsonOut);
 //   myVR::execute(_composite, JSON_Layer_Map2D, jsonOut);	
	return true;
}


// ------------------------------------------------------------------------------------------------
void GLResourcesLoader::initMeshes()
{
	//if( !_cube )
	{
		std::vector< Math::vec3 > pts;
		std::vector< Color  > colors;
		std::vector< Math::vec3 > normals;

		// left
		pts.push_back( Math::vec3( -1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::red().darker() ); normals.push_back( Math::vec3( -1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, -1.0f, 1.0f ) ); colors.push_back( Color::red().darker() ); normals.push_back( Math::vec3( -1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::red().darker() ); normals.push_back( Math::vec3( -1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::red().darker() ); normals.push_back( Math::vec3( -1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::red().darker() ); normals.push_back( Math::vec3( -1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, 1.0f, -1.0f ) ); colors.push_back( Color::red().darker() ); normals.push_back( Math::vec3( -1.0f, 0.0f, 0.0f ) );

		// back
		pts.push_back( Math::vec3( 1.0f, 1.0f, -1.0f ) ); colors.push_back( Color::blue().darker() ); normals.push_back( Math::vec3( 0.0f, 0.0f, -1.0f ) );
		pts.push_back( Math::vec3( -1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f, -1.0f ) );
		pts.push_back( Math::vec3( -1.0f, 1.0f, -1.0f ) ); colors.push_back( Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f, -1.0f ) );
		pts.push_back( Math::vec3( 1.0f, 1.0f, -1.0f ) ); colors.push_back( Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f, -1.0f ) );
		pts.push_back( Math::vec3( 1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f, -1.0f ) );
		pts.push_back( Math::vec3( -1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::blue().darker() );	normals.push_back( Math::vec3( 0.0f, 0.0f, -1.0f ) );

		// bottom
		pts.push_back( Math::vec3( 1.0f, -1.0f, 1.0f ) ); colors.push_back( Color::green().darker() ); normals.push_back( Math::vec3( 0.0f, -1.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::green().darker() ); normals.push_back( Math::vec3( 0.0f, -1.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::green().darker() ); normals.push_back( Math::vec3( 0.0f, -1.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, -1.0f, 1.0f ) ); colors.push_back( Color::green().darker() ); normals.push_back( Math::vec3( 0.0f, -1.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, -1.0f, 1.0f ) ); colors.push_back( Color::green().darker() ); normals.push_back( Math::vec3( 0.0f, -1.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::green().darker() ); normals.push_back( Math::vec3( 0.0f, -1.0f, 0.0f ) );

		// front
		pts.push_back( Math::vec3( -1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f ) );
		pts.push_back( Math::vec3( -1.0f, -1.0f, 1.0f ) ); colors.push_back( Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f ) );
		pts.push_back( Math::vec3( 1.0f, -1.0f, 1.0f ) ); colors.push_back( Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f ) );
		pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f ) );
		pts.push_back( Math::vec3( -1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f ) );
		pts.push_back( Math::vec3( 1.0f, -1.0f, 1.0f ) ); colors.push_back( Color::blue().lighter() ); normals.push_back( Math::vec3( 0.0f, 0.0f, 1.0f ) );

		// right
		pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, 1.0f, -1.0f ) ); colors.push_back( Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, -1.0f, -1.0f ) ); colors.push_back( Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, -1.0f, 1.0f ) ); colors.push_back( Color::red().lighter() );	normals.push_back( Math::vec3( 1.0f, 0.0f, 0.0f ) );

		// top 
		pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, 1.0f, -1.0f ) ); colors.push_back( Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, 1.0f, -1.0f ) ); colors.push_back( Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f ) );
		pts.push_back( Math::vec3( 1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, 1.0f, -1.0f ) ); colors.push_back( Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f ) );
		pts.push_back( Math::vec3( -1.0f, 1.0f, 1.0f ) ); colors.push_back( Color::green().lighter() ); normals.push_back( Math::vec3( 0.0f, 1.0f, 0.0f ) );

		_cube = s2::Renderer::PrimitiveBuffer::New();
		_cube->setVertices( pts );
		_cube->setColors( colors );
		_cube->setNormals( normals );
	}

	{
		//std::vector< Math::vec3 > pts     = { Math::vec3( 0.0f, 0.5f, 0.0f ), Math::vec3( 0.5f, -0.5f, 0.0f ), Math::vec3( -0.5f, -0.5f, 0.0f )};
		//std::vector< Color  > colors      = { Color::green(), Color::blue(), Color::red() };

		//_triangle.setVertices( pts );
		//_triangle.setColors( colors );
	}

	//if( !_torus )
	{
		const Math::Mesh torusMesh = s2::torus( 1.0, 0.65, 64, 16 );
		
		// convert into glmesh
		std::vector<Math::vec3> pts;
		std::vector<Math::vec3> normals;
		for( auto &v : torusMesh.vertices() )
		{
			pts.push_back( v.position );
			normals.push_back( v.normal );
		}

		std::vector<Color> colors( torusMesh.vertices().size(), Color::cyan().transparent(.25) );

		_torus = s2::Renderer::PrimitiveBuffer::New();
		_torus->setVertices( pts );
		_torus->setIndices( torusMesh.indices() );
		_torus->setNormals( normals );
		_torus->setColors( colors );
	}
}

// ------------------------------------------------------------------------------------------------
void GLResourcesLoader::initShaders()
{
	_phong = Renderer::Program::New();
	{
		const bool vsOk = _phong->attachVertexShader( STRINGIFY(
		#version 400\n
		uniform mat4 modelViewProjectionMatrix;
		uniform mat4 modelViewMatrix;
		uniform mat3 normalMatrix;

		in  vec3 in_Vertex;
		in  vec4 in_Color;
		in  vec3 in_Normal;

		out vec3 normalInterp;
		out vec3 vertPos;
		out vec4 color;

		void main()
		{
			gl_Position   = modelViewProjectionMatrix * vec4( in_Vertex, 1.0 );
			vec4 vertPos4 = modelViewMatrix * vec4( in_Vertex, 1.0 );

			vertPos       = vec3( vertPos4 ) / vertPos4.w;
			normalInterp  = normalMatrix * in_Normal;
			color         = in_Color;
		}
		) );

		_phong->attachFragmentShader( STRINGIFY(
		#version 400\n
		uniform vec4  lightPosition;
		uniform vec4  lightAmbient;
		uniform vec4  lightDiffuse;
		uniform vec4  lightSpecular;
		uniform float lightShininess;

		in vec3 normalInterp;
		in vec3 vertPos;
		in vec4 color;

		out vec4 fragColor;

		void main()
		{
			vec3 normal      = normalize( normalInterp );
			vec3 lightDir    = normalize( lightPosition.xyz - vertPos );
			float lambertian = max( dot( lightDir, normal ), 0.0 );
			float specular   = 0.0;

			vec3 viewDir    = normalize( -vertPos );

			vec3 halfDir    = normalize( lightDir + viewDir );
			float specAngle = max( dot( halfDir, normal ), 0.0 );
			specular        = pow( specAngle, lightShininess );

			fragColor = color *
				vec4( lightAmbient +
					  lightDiffuse  * lambertian +
					  lightSpecular * specular );
		} 
		) );

		//_shader->attachVertexShader  ( R::string("Shaders/BlinnPhong.vs") );
		//_shader->attachFragmentShader( R::string("Shaders/BlinnPhong.fs") );
		_phong->link("BlinnPhong");

		std::cout << _phong->info( true );


		_phong->uniform<Math::vec4>( "lightPosition" )->set( Math::vec4( 0, 0, 10, 1 ) );
		_phong->uniform<Math::vec4>( "lightAmbient" )->set( Math::vec4( 0.1, 0.1, 0.1, 1 ) );
		_phong->uniform<Math::vec4>( "lightDiffuse" )->set( Math::vec4( .7, .7, .7, 1 ) );
		_phong->uniform<Math::vec4>( "lightSpecular" )->set( Math::vec4( 1, 1, 1, 1 ) );
		_phong->uniform<float>     ( "lightShininess" )->set( 125.f );

		_phong->uniform<Math::mat4>( "modelViewProjectionMatrix" )->set( Math::mat4( 1 ) );
		_phong->uniform<Math::mat4>( "modelViewMatrix"           )->set( Math::mat4( 1 ) );
		_phong->uniform<Math::mat3>( "normalMatrix"              )->set( Math::mat3( 1 ) );
	}

	_background = Renderer::Program::New();
	{
		const bool vsOk = _background->attachVertexShader( STRINGIFY(
			#version 400\n
			layout(location = 0) in vec3 in_Vertex;
			layout(location = 3) in vec2 in_TexCoord;

			out float gradient;
			out vec2 uv;

			void main() 
			{
				gl_Position = vec4(in_Vertex,1.0);
				gradient = in_Vertex.y * 0.5 + 0.5;
				uv = in_TexCoord;
			}
		) );

		const bool fsOk = _background->attachFragmentShader( STRINGIFY(
			#version 400\n

			uniform vec4 top_color;
			uniform vec4 bot_color;
			
			uniform sampler2D text;

			in vec2  uv;
			in float gradient;
			
			out vec4 frag_color;

			void main()
			{
				frag_color = mix( bot_color, top_color, gradient );
				frag_color = frag_color * texture( text, uv );
				//frag_color = color;
			}
		));

		if( _background->link( "Background" ) )
			std::cout << _background->info( true ) << std::endl;
		else
			_background = nullptr;
	}

	_simpleShader = Renderer::Program::New();
	{

		const bool vsOk = _simpleShader->attachVertexShader( STRINGIFY(
		#version 400\n
		uniform mat4 modelViewProjectionMatrix;

		in vec3 in_Vertex;
		in vec4 in_Color;

		out vec4 color;

		void main()
		{
			gl_Position = modelViewProjectionMatrix * vec4( in_Vertex, 1.0 );
			//color       = in_Color;
			color       = vec4( 1.0, 0.0, 1.0, 1.0 );
		}
		) );


		_simpleShader->attachFragmentShader( STRINGIFY(
		#version 400\n

		in vec4 color;
		out vec4 fragColor;

		void main()
		{
			fragColor = color;
		}
		) );

		_simpleShader->link("Simple");


		std::cout << _simpleShader->info( true ) << std::endl;
	}



	// TEST PIPE SHADER
	_pipeShader = Renderer::Program::New();
	{		
		const bool vsOk = _pipeShader->attachVertexShader( STRINGIFY(
		#version 400\n
		//uniform mat4 modelViewProjectionMatrix;
		//uniform mat4 modelViewMatrix;
		//uniform mat3 normalMatrix;

		in  vec3 in_Vertex;
		in  vec4 in_Color;
		in  vec3 in_Normal;

		out VS_OUT
		{
			vec4 color;
		} vs_out;

		void main()
		{
			gl_Position  = vec4( in_Vertex, 1.0 ); // passthrough worldspace coordinates 
			vs_out.color = in_Color;
		}
		) );

		//********************

		const bool gsOk = _pipeShader->attachGeometryShader( STRINGIFY(
			#version 400\n
			layout( lines) in;
			layout( triangle_strip, max_vertices = 256 ) out;

			in VS_OUT
			{
				vec4 color;
			} gs_in[];


			// pass through 
			out vec3 normalInterp;
			out vec3 vertPos;
			out vec4 color;

			uniform mat4 modelViewProjectionMatrix;
			uniform mat4 modelViewMatrix;
			uniform mat3 normalMatrix;
			uniform int slices;

			const float PI     = 3.1415926535897932384626433832795;

			// project p onto plane (n,0.0)
			void computePlaneProjction( vec4 n, vec4 p, out vec4 prj )
			{
				prj = p - ( ( dot( n, p ) + vec4( 0.0 ) ) * n );
			}

			// build local rotation matrix to align the cylinder to the X local axis
			void buildFrame( vec4 dir, out mat4 rot )
			{
				vec4 x0 = vec4( 1.0, 0.0, 0.0, 0.0 );
				vec4 y0 = vec4( 0.0, 1.0, 0.0, 0.0 );
				vec4 z0 = vec4( 0.0, 0.0, 1.0, 0.0 );

				vec4 pX; computePlaneProjction( dir, x0, pX );
				vec4 pY; computePlaneProjction( dir, y0, pY );
				vec4 pZ; computePlaneProjction( dir, z0, pZ );

				// compute lenght of each projection
				float xLen = length( pX );
				float yLen = length( pY );
				float zLen = length( pZ );

				vec4 ortho;
				if( xLen > yLen )
				{
					if( xLen > zLen ) ortho = pX;
					else              ortho = pZ;
				}
				else
				{
					if( yLen > zLen ) ortho = pY;
					else              ortho = pZ;
				}

				rot[0] = dir;
				rot[1] = normalize( ortho );
				rot[2] = vec4( normalize( cross( vec3(dir), vec3(ortho) ) ), 0.0 );
				rot[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
			}
		
			void main()
			{
				const int slices   = 32;
				const float radius = 1.0;
				vec4 p0            = gl_in[0].gl_Position;
				vec4 p1            = gl_in[1].gl_Position;
				float len          = length( p1 - p0 );
				vec4 dir           = normalize( p1 - p0 );

				mat4 frame; buildFrame( dir, frame );

				const float dTheta = 2.0 * PI / float( slices );

				for( int s=0; s < slices+1; ++s )
				{
					float theta = s * dTheta;
					float ct    = cos( theta );
					float st    = sin( theta );

					vec4 startPoint = p0 + ( frame * vec4( 0.0, ct*radius, st*radius, 1.0 ) );
					vec4 endPoint   = ( startPoint + dir * len );
					vec4 pos;

					gl_Position  = modelViewProjectionMatrix * endPoint;
					pos          = modelViewMatrix * endPoint;
					vertPos      = vec3( pos) / pos.w;
					normalInterp = normalMatrix * mat3(frame) * vec3( 0.0, ct*radius, st*radius );					
					color        = gs_in[1].color;
					EmitVertex();


					gl_Position  = modelViewProjectionMatrix * startPoint;
					pos          = modelViewMatrix * startPoint;
					vertPos      = vec3( pos) / pos.w;
					normalInterp = normalMatrix * mat3(frame) * vec3( 0.0, ct*radius, st*radius );					
					color        = gs_in[0].color;					
					EmitVertex();
				}
				EndPrimitive();
				
				// start sphere caps...
			}
		) );

		_pipeShader->attachFragmentShader( STRINGIFY(
		#version 400\n
		uniform vec4  lightPosition;
		uniform vec4  lightAmbient;
		uniform vec4  lightDiffuse;
		uniform vec4  lightSpecular;
		uniform float lightShininess;

		in vec3 normalInterp;
		in vec3 vertPos;
		in vec4 color;

		out vec4 fragColor;

		void main()
		{
			vec3 normal      = normalize( normalInterp );
			vec3 lightDir    = normalize( lightPosition.xyz - vertPos );
			float lambertian = max( dot( lightDir, normal ), 0.0 );
			float specular   = 0.0;

			vec3 viewDir    = normalize( -vertPos );

			vec3 halfDir    = normalize( lightDir + viewDir );
			float specAngle = max( dot( halfDir, normal ), 0.0 );
			specular        = pow( specAngle, lightShininess );

			fragColor = color *
				vec4( lightAmbient +
					  lightDiffuse  * lambertian +
					  lightSpecular * specular );
		} 
		) );

		_pipeShader->link("PipeShader");

		std::cout << _pipeShader->info( true );


		_pipeShader->uniform<Math::vec4>( "lightPosition" )->set( Math::vec4( 0, 0, 10, 1 ) );
		_pipeShader->uniform<Math::vec4>( "lightAmbient" )->set( Math::vec4( 0.1, 0.1, 0.1, 1 ) );
		_pipeShader->uniform<Math::vec4>( "lightDiffuse" )->set( Math::vec4( .7, .7, .7, 1 ) );
		_pipeShader->uniform<Math::vec4>( "lightSpecular" )->set( Math::vec4( 1, 1, 1, 1 ) );
		_pipeShader->uniform<float>     ( "lightShininess" )->set( 125.f );

		_pipeShader->uniform<Math::mat4>( "modelViewProjectionMatrix" )->set( Math::mat4( 1 ) );
		_pipeShader->uniform<Math::mat4>( "modelViewMatrix"           )->set( Math::mat4( 1 ) );
		_pipeShader->uniform<Math::mat3>( "normalMatrix"              )->set( Math::mat3( 1 ) );
	}
}