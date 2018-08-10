// TestScene.cpp
//
#include "TestScene.h"

#include "GLResourcesLoader.h"

#include "qt/UserInteractionManager.h"
#include "qt/MouseStatus.h"

#include "opengl/Extensions.h"
#include "utils/String.h"
#include "utils/TimedBlock.h"
#include "resources/R.h"

#include "math/Ray.h"
#include "math/Plane.h"
#include "math/Mesh.h"

#include <QPainter>
#include <QTimer>
#include <iostream>

using namespace s2;

// ------------------------------------------------------------------------------------------------
TestScene::TestScene( QWidget *parent )
: GLWidget( parent )
, _wireframe( false )
, _frames( 0 )
{
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "Wheel",                [&] () { onMouseWheel(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "LeftButton",           [&] () { onMousePressed(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "RightButton",          [&] () { onMousePressed(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "RightButton+Released", [&] () { onMouseReleased(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "LeftButton+Drag",      [&] () { onMouseMoved(); } ) );
	_uim.registerMouseCommand( s2::Qt::UIMCommand( "RightButton+Drag",     [&] () { onMouseMoved(); } ) );

	resetView();
}

// ------------------------------------------------------------------------------------------------
TestScene::~TestScene()
{
	makeCurrent();
	_surface = nullptr;
	_shader = nullptr;
	_meshes.clear();
	doneCurrent();
}

// ------------------------------------------------------------------------------------------------
void TestScene::addMesh( const s2::OpenGL::MeshPtr &m )
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
	_center = Math::dvec3( 0, 0, 0 );
	_camera.set( Math::dvec3( 0, 0, 4 ), Math::dvec3( 0, 0, 0 ), Math::dvec3( 0, 1, 0 ) );

	_viewState.setModelMatrix( Math::dmat4( 1 ) );
	_viewState.setViewMatrix( _camera.matrix() );
	_viewState.setProjectionMatrix( Math::perspective( 45.0, 4.0 / 3, 0.1, 100.0 ) );
}

// ------------------------------------------------------------------------------------------------
void TestScene::initializeGL()
{
	s2::OpenGL::initExtensions();
	_surface = s2::OpenGL::Surface::New();

	GLResourcesLoader::_background->uniform < Math::vec4 >("top_color")->set( Color::gray().lighter(.25) );
	GLResourcesLoader::_background->uniform < Math::vec4 >("bot_color")->set( Color::gray().darker(.25) );
}

// ------------------------------------------------------------------------------------------------
void TestScene::initFonts()
{
	//s2::FontsLibrary::init(L"./res/fonts");
}

// ------------------------------------------------------------------------------------------------
void TestScene::paintGL()
{
	QPainter painter;
	//TimedBlock total( "total grid time" );

#if 1
	painter.beginNativePainting();

	OpenGL::ClearState clear;
	clear.color = Color::gray();

	_surface->clear( clear );
	
	{
		OpenGL::DrawingState ds( GLResourcesLoader::_background );
		ds.renderState.depthTest.enabled = false;

		std::vector<s2::Math::vec3> vertices = { {-1,1,0},{-1,-1,0},{1,1,0},{1,-1,0} };

		
		OpenGL::Mesh m;
		m.setVertices( vertices );

	
		_surface->draw( OpenGL::Primitive::TriangleStrip, m.vao(), ds );
	}


	OpenGL::DrawingState ds( _shader );	
	ds.renderState.blending.enabled                = true;
	ds.renderState.blending.sourceRGBFactor        = OpenGL::Blending::Factor::SourceAlpha;
	ds.renderState.blending.sourceAlphaFactor      = OpenGL::Blending::Factor::SourceAlpha;
	ds.renderState.blending.destinationRGBFactor   = OpenGL::Blending::Factor::OneMinusSourceAlpha;
	ds.renderState.blending.destinationAlphaFactor = OpenGL::Blending::Factor::OneMinusSourceAlpha;

	OpenGL::ViewState vs( _viewState );

	{
		// 88msec
		const int g = 11;
		for( int i=0; i < g; ++i )
		{
			for( int j=0; j < g; j++ )
			{
				//if( i == 2 && j == 2 )
				{
					//TimedBlock single("    SINGLE TIME");
					Math::dmat4 m = Math::translate( Math::dmat4( 1.0 ), Math::dvec3( 4 * i - g*.5f, 4 * j - g*.5f, 0 ) );

					vs.setModelMatrix( m*_viewState.modelMatrix() );

					_shader->uniform<Math::mat4>( "modelViewProjectionMatrix" )->set( vs.modelViewProjectionMatrix() );
					_shader->uniform<Math::mat4>( "modelViewMatrix" )->set( vs.modelViewMatrix() );
					_shader->uniform<Math::mat3>( "normalMatrix" )->set( vs.normalMatrix() );


					OpenGL::MeshPtr mesh = (j+i*g)%2 == 0 ? GLResourcesLoader::_cube : GLResourcesLoader::_torus;

					//OpenGL::MeshPtr mesh = GLResourcesLoader::_cube;
					_surface->draw( OpenGL::Primitive::Triangles, mesh->vao(), ds );
				}
				//else
				//{
				//	Math::dmat4 m = Math::translate( Math::dmat4( 1.0 ), Math::dvec3( 4 * i - g*.5f, 4 * j - g*.5f, 0 ) );

				//	vs.setModelMatrix( m*_viewState.modelMatrix() );

				//	_shader->uniform<Math::mat4>( "modelViewProjectionMatrix" )->set( vs.modelViewProjectionMatrix() );
				//	_shader->uniform<Math::mat4>( "modelViewMatrix" )->set( vs.modelViewMatrix() );
				//	_shader->uniform<Math::mat3>( "normalMatrix" )->set( vs.normalMatrix() );

				//	OpenGL::MeshPtr mesh = GLResourcesLoader::_cube;
				//	_surface->draw( OpenGL::Primitive::Triangles, mesh->vao(), ds );
				//}
			}
		}
	}
	_surface->swap( defaultFramebufferObject() );
	painter.endNativePainting();
#endif

#if 1
	painter.begin( this );
	if( const int elapsed = _time.elapsed() )
	{
		QString framesPerSecond;
		framesPerSecond.setNum( _frames / ( elapsed / 1000.0 ), 'f', 2 );
		painter.setPen( ::Qt::white );
		painter.drawText( 10, 40, framesPerSecond + " paintGL calls / s" );
	}

	if( !( _frames % 100 ) ) {
		_time.start();
		_frames = 0;
	}
	painter.end();
	++_frames;
#endif
	update();
	//QTimer::singleShot( 100, [this] () {  update(); } );
}

// ------------------------------------------------------------------------------------------------
void TestScene::updateAnimations()
{
	static double angle = 0.0;


	const Math::dmat4 m = 
		Math::rotate( Math::dmat4( 1 ), Math::radians( angle * .5 ), Math::dvec3( 1, 1, 0 ) ) *
		Math::rotate( Math::dmat4( 1 ), Math::radians( angle * .3 ), Math::dvec3( 0, 1, 0 ) ) *
		Math::rotate( Math::dmat4( 1 ), Math::radians( angle * .9 ), Math::dvec3( 0, 0, 1 ) )
		;

	angle = angle+1.0;
	_viewState.setModelMatrix( m );
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
		_trackball.update( s2::Renderer::TrackBall::Start_Drag, Math::ivec2( ms.currentPosition().x, ms.currentPosition().y ) );
#endif
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseReleased()
{
#if 1
	const s2::Qt::MouseStatus ms = _uim.mouseStatus();

	if( ms.buttonUp() & s2::Qt::MouseStatus::ButtonRight )
		_trackball.update( s2::Renderer::TrackBall::End_Drag, Math::ivec2( ms.currentPosition().x, ms.currentPosition().y ) );
#endif
}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseMoved()
{
	const s2::Qt::MouseStatus ms = _uim.mouseStatus();

#if 0
	if( ms.buttonDown() & s2::Qt::MouseStatus::ButtonLeft )
	{
		Math::dvec3 p0 = _viewState.worldPoint( ms.previousPosition().x, ms.previousPosition().y );
		Math::dvec3 p1 = _viewState.worldPoint( ms.currentPosition().x, ms.currentPosition().y );
		Math::dvec3 d  = ( p1 - p0 )*100.0;
		//Math::dvec3 d  = (p1-p0) * Math::min( 50.0, 1.0/Math::determinant( _viewState.model ) );

		_camera.set(
			_camera.position() - d,
			_camera.target() - d,
			_camera.upVector()
		);
	}
#endif

	if( ms.buttonDown() & s2::Qt::MouseStatus::ButtonRight )
		_trackball.update( s2::Renderer::TrackBall::Drag, Math::ivec2( ms.currentPosition().x, ms.currentPosition().y ) );

	Math::dmat4 m = Math::scale( Math::dmat4( 1 ), Math::dvec3( 1.0 ) ) *
		Math::translate( Math::dmat4( 1 ), _camera.target() )*
		_trackball.matrix() *
		Math::translate( Math::dmat4( 1 ), -_camera.target() )
		;

	_viewState.setViewMatrix( _camera.matrix()  *  m );

}

// -----------------------------------------------------------------------------------------------
void TestScene::onMouseWheel()
{
#if 1
	const s2::Qt::MouseStatus ms = _uim.mouseStatus();
	const double z               = ms.wheel();

	//_viewState.setModelMatrix( Math::scale( _viewState.modelMatrix(), Math::dvec3( z ) ) );

	_camera.set( _camera.position()+Math::dvec3(0,0,z), _camera.target(), _camera.upVector() );
	_viewState.setViewMatrix( _camera.matrix() * _trackball.matrix() );

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
	_camera.set( Math::dvec3( 0, 0, 4 ), Math::dvec3( 0, 0, 0 ), Math::dvec3( 0, 1, 0 ) );
	_viewState.model = Math::dmat4();
#endif
	_trackball.reset();
}

// -----------------------------------------------------------------------------------------------
void TestScene::resizeGL( int w, int h )
{
	_viewState.setViewport( Math::Rectangle( 0, 0, w, h ) );
	if( _surface )
		_surface->resize( w, h );

	_viewState.setProjectionMatrix( Math::perspective( 45.0, w / (double) h, 0.1, 100.0 ) );
	_trackball.resize( w, h );



#if 0	
	_viewState.view.setAspectRatio( w / (double) h );
	_viewState.view.setPerspective( w / (double) h, 45.0, 0.1, 1000 );

	_trackball.resize( w, h );
#endif
}

//// -----------------------------------------------------------------------------------------------
//void TestScene::refreshScene()
//{
//
//}