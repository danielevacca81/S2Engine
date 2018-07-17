// TestScene.cpp
//
#include "TestScene.h"

#include "qt/UserInteractionManager.h"
#include "qt/MouseStatus.h"

#include "opengl/Extensions.h"
#include "utils/String.h"
#include "resources/R.h"

#include "math/Ray.h"
#include "math/Plane.h"

#include "math/Mesh.h"


#include <QPainter>
#include <iostream>

using namespace s2;

// ------------------------------------------------------------------------------------------------
TestScene::TestScene( QWidget *parent )
: GLGraphicsScene( parent )
, _wireframe(false)
//, _viewState( OpenGL::ViewState::Perspective )
//, _fonts("")
{
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "Zoom",          "Wheel",               [&] () { onMouseWheel(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "MouseLPress",   "LeftButton",          [&] () { onMousePressed(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "MouseRPress",   "RightButton",         [&] () { onMousePressed(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "MouseRRelease", "RightButton+Released",[&] () { onMouseReleased(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "Pan",           "LeftButton+Drag",     [&] () { onMouseMoved(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "Rotate",        "RightButton+Drag",    [&] () { onMouseMoved(); } ) );

	resetView();
}

// ------------------------------------------------------------------------------------------------
TestScene::~TestScene()
{}

// ------------------------------------------------------------------------------------------------
void TestScene::addMesh( const s2::OpenGL::Mesh &m )
{
	_meshes.push_back( m );
}

// ------------------------------------------------------------------------------------------------
void TestScene::setShader( const s2::OpenGL::ProgramPtr &shader )
{
	_shader = shader;
}

// ------------------------------------------------------------------------------------------------
void TestScene::resetView()
{
	_center = Math::dvec3(0,0,0);
	_camera.set( Math::dvec3(0,0,4), Math::dvec3(0,0,0), Math::dvec3(0,1,0));
	
	_viewState.setModelMatrix( Math::dmat4( 1 ) );
	_viewState.setViewMatrix( _camera.matrix() );
	_viewState.setProjectionMatrix( Math::perspective(45.0,4.0/3,0.1,100.0) );
}

// ------------------------------------------------------------------------------------------------
void TestScene::initializeOpenGL()
{
	_fb = s2::OpenGL::FrameBuffer::getDefault();
	std::cout << _fb->info() << std::endl;
}

// ------------------------------------------------------------------------------------------------
void TestScene::initFonts()
{
	//s2::FontsLibrary::init(L"./res/fonts");
}

// ------------------------------------------------------------------------------------------------
void TestScene::renderScene()
{
	OpenGL::ClearState clear;
	clear.color = Color::gray();
	_fb->clear( clear );

	Math::dmat4 m = Math::scale( Math::dmat4(1),Math::dvec3(1.0)) * 
		            Math::translate( Math::dmat4(1), _camera.target() )*
					_trackball.matrix() *
					Math::translate( Math::dmat4(1), -_camera.target() )
					;
	
	_viewState.setModelMatrix( Math::dmat4(1.0) );
	_viewState.setViewMatrix( _camera.matrix()  *  m  );

	if( _shader )
	{
		_shader->uniform<Math::mat4>( "modelViewProjectionMatrix" )->set( _viewState.modelViewProjectionMatrix() );
		_shader->uniform<Math::mat4>( "modelViewMatrix"           )->set( _viewState.modelViewMatrix() );
		_shader->uniform<Math::mat3>( "normalMatrix"              )->set( _viewState.normalMatrix() );


		OpenGL::DrawState ds( _shader );
		for( auto &mesh : _meshes )
			_fb->draw( OpenGL::Primitive::Triangles, mesh, ds );
	}

/*
	{
		_shaderBlinnPhong->uniform<Math::mat4>( "modelViewProjectionMatrix" )->set( _viewState.modelViewProjectionMatrix() );
		_shaderBlinnPhong->uniform<Math::mat4>( "modelViewMatrix"           )->set( _viewState.modelViewMatrix() );
		_shaderBlinnPhong->uniform<Math::mat3>( "normalMatrix"              )->set( _viewState.normalMatrix() );

		OpenGL::DrawState ds( _shaderBlinnPhong );
		_fb->draw( OpenGL::Primitive::Triangles, _cubeMesh, ds );
	}

	//{
	//	OpenGL::DrawState ds( _shaderSimple );
	//	ds.renderState.faceCulling.enabled = false;
	//	ds.renderState.depthTest.enabled   = false;

	//	_fb->draw( OpenGL::Primitive::Triangles, _triangle, ds );
	//}


	{
		OpenGL::DrawState ds( _shaderBlinnPhong );
		ds.renderState.blending.enabled = true;
	
		_fb->draw( OpenGL::Primitive::Triangles, _torus, ds );
	}
*/
}

// ------------------------------------------------------------------------------------------------
void TestScene::toggleWireframe()
{
	_wireframe = !_wireframe;
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMousePressed()
{
#if 1
	const s2::Qt::MouseStatus ms = _uim.mouseStatus();

	if( ms.buttonDown() & s2::Qt::MouseStatus::ButtonRight )
		_trackball.update( s2::Renderer::TrackBall::Start_Drag, Math::ivec2( ms.currentPosition().x,ms.currentPosition().y ) );
#endif
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseReleased()
{
#if 1
	const s2::Qt::MouseStatus ms = _uim.mouseStatus();
	
	if( ms.buttonUp() & s2::Qt::MouseStatus::ButtonRight )
		_trackball.update( s2::Renderer::TrackBall::End_Drag, Math::ivec2( ms.currentPosition().x,ms.currentPosition().y ) );
#endif
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseMoved()
{
	const s2::Qt::MouseStatus ms = _uim.mouseStatus();

#if 0
	if( ms.buttonDown() & s2::Qt::MouseStatus::ButtonLeft )
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
#endif

	if( ms.buttonDown() & s2::Qt::MouseStatus::ButtonRight )
		_trackball.update( s2::Renderer::TrackBall::Drag, Math::ivec2( ms.currentPosition().x,ms.currentPosition().y ) );
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseWheel()
{
#if 0
	const s2::Qt::MouseStatus ms = _uim.mouseStatus();
	const double z               = ms.wheel();

	_viewState.model = Math::scale( _viewState.model, Math::dvec3(z) );

	//_viewState.camera.zoomToTarget(1.5);
	//const Math::dvec3 pos( currentWorldPoint.x, currentWorldPoint.y, 0.0 );

	//renderer->zoomInOut( z );

	//// get the center of the scene
	//Math::dvec3 target( _camera->center() );
	//Math::dvec3 center( _camera->center() );

	//target = pos;
	//_camera->lookAt( (center - target)/Math::dvec3(z) + target ); //compute the new center position
#endif
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseDoubleClick()
{
	const s2::Qt::MouseStatus ms = _uim.mouseStatus();
#if 0
	_camera.set( Math::dvec3(0,0,4), Math::dvec3(0,0,0), Math::dvec3(0,1,0));
	_viewState.model = Math::dmat4();
#endif
	_trackball.reset();
}

// -----------------------------------------------------------------------------------------------
void TestScene::resizeScene( int w, int h )
{
	//_viewState.setViewport( Math::Rectangle( 0, 0, w, h ) );
	if( _fb )
		_fb->setViewport( Math::Rectangle( 0, 0, w, h ) );

	_viewState.setProjectionMatrix( Math::perspective(45.0,w/(double)h,0.1,100.0) );
	_trackball.resize( w,h );



#if 0	
	_viewState.view.setAspectRatio( w/(double)h );
	_viewState.view.setPerspective( w/(double)h, 45.0, 0.1, 1000 );
	
	_trackball.resize( w,h );
#endif
}

// -----------------------------------------------------------------------------------------------
void TestScene::refreshScene()
{

}