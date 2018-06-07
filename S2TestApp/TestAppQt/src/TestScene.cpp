// TestScene.cpp
//
#include "TestScene.h"

//#include "VObjects/VLine.h"
//#include "VObjects/VCircle.h"

#include "qtbridge/UserInteractionManager.h"
#include "qtbridge/MouseStatus.h"

//#include "Renderer/TextRenderer.h"
#include "opengl/Extensions.h"
#include "utils/String.h"
//#include "Fonts/FontNew.h"

//#include "OpenGL/Uniform.h"

//#include "Common/StringTools.h"
#include "Resources/R.h"

#include "Math/Ray.h"
#include "Math/Plane.h"

//#include "Graphics/Font.h"

#include <QPainter>
#include <iostream>

// ------------------------------------------------------------------------------------------------
TestScene::TestScene( QWidget *parent )
: GLGraphicsScene( parent )
, _wireframe(false)
, _viewState( OpenGL::ViewState::Perspective )
//, _fonts("")
{
	_uim.registerMouseCommand( QtBridge::UIMCommand( "Zoom",         "Wheel",               [=] () { onMouseWheel(); } ) );
	_uim.registerMouseCommand( QtBridge::UIMCommand( "MouseLPress",   "LeftButton",         [=] () { onMousePressed(); } ) );
	_uim.registerMouseCommand( QtBridge::UIMCommand( "MouseRPress",   "RightButton",        [=] () { onMousePressed(); } ) );
	_uim.registerMouseCommand( QtBridge::UIMCommand( "MouseRRelease", "RightButton+Released",[=] () { onMouseReleased(); } ) );
	_uim.registerMouseCommand( QtBridge::UIMCommand( "Pan",           "LeftButton+Drag",     [=] () { onMouseMoved(); } ) );
	_uim.registerMouseCommand( QtBridge::UIMCommand( "Rotate",        "RightButton+Drag", [=] () { onMouseMoved(); } ) );
}

// ------------------------------------------------------------------------------------------------
TestScene::~TestScene()
{
	//delete _objectManager;
	//delete _cadEngine;
}

// ------------------------------------------------------------------------------------------------
void TestScene::postInitialization()
{
//	s2::FontsLibrary::init(L"./fonts");
	
	//_textRenderer.init();

	//_textRenderer.addTag( s2::TextRenderer::TextTag( Math::ivec2(0,0),L"Watashi: わたし - 私",Color::red(),"Arial.ttf",0, s2::TextRenderer::AlignLeft) );
	
	_center = Math::dvec3(0,0,0);
	_camera.set( Math::dvec3(0,0,4), Math::dvec3(0,0,0), Math::dvec3(0,1,0));
	_viewState.model = Math::dmat4( 1 );

	createMeshes();
	createObjects();
}

// ------------------------------------------------------------------------------------------------
void TestScene::createObjects()
{
	//s2::VLinePtr line = s2::VLine::New( Math::dvec3(0,0,0), Math::dvec3(5,5,5) );
	//
	//line->setName( "Original" );

	//_objectManager.addObject(line);

	//line->setName( "Renamed" );
	//line->setState( s2::VObject::State() );
}

// ------------------------------------------------------------------------------------------------
void TestScene::createMeshes()
{
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

		_cubeMesh.setVertices( pts );
		_cubeMesh.setColors( colors );
		_cubeMesh.setNormals( normals );

	}

	{
		std::vector< Math::vec3 > pts     = { Math::vec3( 0.0f, 0.5f, 0.0f ), Math::vec3( 0.5f, -0.5f, 0.0f ), Math::vec3( -0.5f, -0.5f, 0.0f )};
		std::vector< Color  > colors      = { Color::green(), Color::green(), Color::green() };
		//std::vector< Math::vec3 > normals = { Math::vec3( 0.0f, 0.0f, 1.0f ), Math::vec3( 0.0f, 0.0f, 1.0f ), Math::vec3( 0.0f, 0.0f, 1.0f )};

		_triangle.setVertices( pts );
		_triangle.setColors( colors );
		//_triangle.setNormals( normals );
	}
}


// ------------------------------------------------------------------------------------------------
void TestScene::initializeOpenGL()
{
	OpenGL::initExtensions();
	std::cout << OpenGL::contextInfo() << std::endl;


	initShaders();

	postInitialization();

	//_context.initExtensions();
	//std::cout << _context.info() << std::endl;
}

// ------------------------------------------------------------------------------------------------
void TestScene::initShaders()
{
	_shaderBlinnPhong = OpenGL::Program::New();
	{
		const bool vsOk = _shaderBlinnPhong->attachVertexShader( STRINGIFY(
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

		_shaderBlinnPhong->attachFragmentShader( STRINGIFY(
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
		_shaderBlinnPhong->link("BlinnPhong");

		std::cout << _shaderBlinnPhong->info( true );


		_shaderBlinnPhong->uniform<Math::vec4>( "lightPosition" )->setValue( Math::vec4( 0, 0, 10, 1 ) );
		_shaderBlinnPhong->uniform<Math::vec4>( "lightAmbient" )->setValue( Math::vec4( 0, 0, 0, 1 ) );
		_shaderBlinnPhong->uniform<Math::vec4>( "lightDiffuse" )->setValue( Math::vec4( 1, 1, 1, 1 ) );
		_shaderBlinnPhong->uniform<Math::vec4>( "lightSpecular" )->setValue( Math::vec4( 1, 1, 0, 1 ) );
		_shaderBlinnPhong->uniform<float>     ( "lightShininess" )->setValue( 127.f );

		_shaderBlinnPhong->uniform<Math::mat4>( "modelViewProjectionMatrix" )->setValue( Math::mat4( 1 ) );
		_shaderBlinnPhong->uniform<Math::mat4>( "modelViewMatrix"           )->setValue( Math::mat4( 1 ) );
		_shaderBlinnPhong->uniform<Math::mat3>( "normalMatrix"              )->setValue( Math::mat3( 1 ) );
	}

	_shaderSimple = OpenGL::Program::New();
	{

		const bool vsOk = _shaderSimple->attachVertexShader( STRINGIFY(
		#version 400\n

		in vec3 in_Vertex;
		in vec4 in_Color;

		out vec4 color;

		void main()
		{
			gl_Position   = vec4( in_Vertex, 1.0 );

			color = in_Color;
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

	}


}

// ------------------------------------------------------------------------------------------------
void TestScene::initFonts()
{
	//s2::FontsLibrary::init(L"./res/fonts");
}

// ------------------------------------------------------------------------------------------------
void TestScene::renderScene()
{
	OpenGL::ClearState cs;
	cs.color = Color::gray();
	_renderer.clear( cs );

	Math::dmat4 m = Math::scale( Math::dmat4(1),Math::dvec3(1.0)) * 
		            Math::translate( Math::dmat4(1), _camera.target() )*
					_trackball.matrix() *
					Math::translate( Math::dmat4(1), -_camera.target() )
					;
	
	_viewState.view.set( _camera.matrix()  *  m );

	{
		_shaderBlinnPhong->uniform<Math::mat4>( "modelViewProjectionMatrix" )->setValue( _viewState.projectionMatrix() * _viewState.modelViewMatrix() );
		_shaderBlinnPhong->uniform<Math::mat4>( "modelViewMatrix"           )->setValue( _viewState.modelViewMatrix() );
		_shaderBlinnPhong->uniform<Math::mat3>( "normalMatrix"              )->setValue( _viewState.normalMatrix() );

		OpenGL::DrawState ds( _shaderBlinnPhong );

		_renderer.draw( OpenGL::Triangles, _cubeMesh, _viewState, ds );
	}

	//{
	//	OpenGL::DrawState ds( _shaderSimple );
	//	ds.renderState.faceCulling.enabled = false;
	//	ds.renderState.depthTest.enabled   = false;

	//	_renderer.draw( OpenGL::Triangles, _triangle, _viewState, ds );
	//}
}

// ------------------------------------------------------------------------------------------------
void TestScene::toggleWireframe()
{
	_wireframe = !_wireframe;
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMousePressed()
{
	const QtBridge::MouseStatus ms = _uim.mouseStatus();

	if( ms.buttonDown() & QtBridge::MouseStatus::ButtonRight )
		_trackball.update( s2::TrackBall::Start_Drag, Math::ivec2( ms.currentPosition().x,ms.currentPosition().y ) );
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseReleased()
{
	const QtBridge::MouseStatus ms = _uim.mouseStatus();
	
	if( ms.buttonUp() & QtBridge::MouseStatus::ButtonRight )
		_trackball.update( s2::TrackBall::End_Drag, Math::ivec2( ms.currentPosition().x,ms.currentPosition().y ) );
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseMoved()
{
	const QtBridge::MouseStatus ms = _uim.mouseStatus();

	if( ms.buttonDown() & QtBridge::MouseStatus::ButtonLeft )
	{
		Math::dvec3 p0 = _viewState.worldPoint( ms.previousPosition().x, ms.previousPosition().y);
		Math::dvec3 p1 = _viewState.worldPoint( ms.currentPosition().x,  ms.currentPosition().y );
		Math::dvec3 d  = (p1-p0)*100.0;		
		//Math::dvec3 d  = (p1-p0) * Math::min( 50.0, 1.0/Math::determinant( _viewState.model ) );

		_camera.set( 
			_camera.position()-d,
			_camera.target()-d,
			_camera.upVector()
			);
	}

	if( ms.buttonDown() & QtBridge::MouseStatus::ButtonRight )
		_trackball.update( s2::TrackBall::Drag, Math::ivec2( ms.currentPosition().x,ms.currentPosition().y ) );
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseWheel()
{
	const QtBridge::MouseStatus ms = _uim.mouseStatus();
	const double z                 = ms.wheel();

	_viewState.model = Math::scale( _viewState.model, Math::dvec3(z) );

	//_viewState.camera.zoomToTarget(1.5);
	//const Math::dvec3 pos( currentWorldPoint.x, currentWorldPoint.y, 0.0 );

	//renderer->zoomInOut( z );

	//// get the center of the scene
	//Math::dvec3 target( _camera->center() );
	//Math::dvec3 center( _camera->center() );

	//target = pos;
	//_camera->lookAt( (center - target)/Math::dvec3(z) + target ); //compute the new center position




}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseDoubleClick()
{
	const QtBridge::MouseStatus ms = _uim.mouseStatus();
	_camera.set( Math::dvec3(0,0,4), Math::dvec3(0,0,0), Math::dvec3(0,1,0));
	_viewState.model = Math::dmat4();
	_trackball.reset();
}

// -----------------------------------------------------------------------------------------------
void TestScene::resizeScene( int w, int h )
{
	_viewState.viewport = Math::Rectangle(0,0, w,h);
	
	_viewState.view.setAspectRatio( w/(double)h );
	_viewState.view.setPerspective( w/(double)h, 45.0, 0.1, 1000 );
	
	_trackball.resize( w,h );
}

// -----------------------------------------------------------------------------------------------
void TestScene::refreshScene()
{

}