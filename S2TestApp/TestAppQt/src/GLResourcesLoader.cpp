#include "GLResourcesLoader.h"

#include "opengl/Extensions.h"
#include "opengl/Program.h"
#include "opengl/Context.h"

#include "math/Mesh.h"
#include "math/Geometry.h"
#include "utils/String.h"

#include <iostream>

s2::OpenGL::MeshPtr    GLResourcesLoader::_torus;
s2::OpenGL::MeshPtr    GLResourcesLoader::_cube;
s2::OpenGL::ProgramPtr GLResourcesLoader::_phong;
s2::OpenGL::ProgramPtr GLResourcesLoader::_background;
s2::OpenGL::ContextPtr GLResourcesLoader::_mainContext;

using namespace s2;

// ------------------------------------------------------------------------------------------------
GLResourcesLoader::GLResourcesLoader( QWidget *parent )
: QOpenGLWidget(parent)
{
	//create();
	//load();
}


// ------------------------------------------------------------------------------------------------
GLResourcesLoader::~GLResourcesLoader()
{
	//_mainContext->release();
	_mainContext = nullptr;

	makeCurrent();
	//_phong = nullptr;
	_cube = nullptr;
	_torus = nullptr;
	//_background = nullptr;
	doneCurrent();
}

 //------------------------------------------------------------------------------------------------
void GLResourcesLoader::initializeGL()
{
	hide();
	
	_mainContext = OpenGL::Context::Current(); // initializes extensions
	
	std::cout << "Context ID: " << _mainContext->id() << std::endl 
		<< _mainContext->info() << std::endl;

	load();
	emit resourcesInitialized();
}

// ------------------------------------------------------------------------------------------------
bool GLResourcesLoader::load()
{
	initMeshes();
	initShaders();
	return true;
}

// ------------------------------------------------------------------------------------------------
void GLResourcesLoader::initMeshes()
{
	if( !_cube )
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

		_cube = s2::OpenGL::Mesh::New();
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

	if( !_torus )
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

		_torus = s2::OpenGL::Mesh::New();
		_torus->setVertices( pts );
		_torus->setIndices( torusMesh.indices() );
		_torus->setNormals( normals );
		_torus->setColors( colors );
	}
}

// ------------------------------------------------------------------------------------------------
void GLResourcesLoader::initShaders()
{
	_phong = OpenGL::Program::New();
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
		} ) );

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

	_background = OpenGL::Program::New();
	{
		const bool vsOk = _background->attachVertexShader( STRINGIFY(
			#version 400\n
			layout(location = 0) in vec3 in_Vertex;

			out float gradient;

			void main() 
			{
				gl_Position = vec4(in_Vertex,1.0);
				gradient = in_Vertex.y * 0.5 + 0.5;
			}
		) );

		_background->attachFragmentShader( STRINGIFY(
			#version 400\n

			uniform vec4 top_color;
			uniform vec4 bot_color;
			
			in float gradient;
			
			out vec4 frag_color;

			void main()
			{
				frag_color = mix( bot_color, top_color, gradient );
				//frag_color = color;
			}
		) );

		_background->link("Background");


		std::cout << _background->info( true ) << std::endl;	
	}

/*	_shaderSimple = OpenGL::Program::New();
	{

		const bool vsOk = _shaderSimple->attachVertexShader( STRINGIFY(
		#version 400\n

		in vec3 in_Vertex;
		in vec4 in_Color;

		out vec4 color;

		void main()
		{
			gl_Position = vec4( in_Vertex, 1.0 );
			color       = in_Color;
		}
		) );


		_shaderSimple->attachFragmentShader( STRINGIFY(
			#version 400\n

			in vec4 color;
			out vec4 fragColor;

		void main()
		{
			fragColor = color;
		}
		) );

		_shaderSimple->link("Simple");


		std::cout << _shaderSimple->info( true ) << std::endl;
	}*/
}