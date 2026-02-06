// MainWindow.cpp
//
#include "MainWindow.h"

#include "MyApplication.h"

#include "Application/Application.h"

#include "RenderCore/RenderTarget.h"
#include "RenderCore/RenderCore.h"
#include "Core/VectorCast.h"

#include "Geometry/GeometryFactory3D.h"

////// TODO:
// - add keyboard handling
// - computeshaders and opengl 4.6

// ------------------------------------------------------------------------------------------------
void MainWindow::onInitializeEvent()
{
	_planeXZ = RenderCore::VertexData::New();
	_planeXZ->setVertices( std::vector<Math::vec3> {
		    { -2.5f, -2.5f, 0.f },
			{  2.5f, -2.5f, 0.f },
			{  2.5f,  2.5f, 0.f },
			{ -2.5f,  2.5f, 0.f }
	} );
	_planeXZ->setNormals( std::vector<Math::vec3>( 4, { 0.f, 0.f, 1.f } ) );
	_planeXZ->setIndices( { 0, 1, 2, 0, 2, 3 } );
	_planeXZ->setColors( std::vector<Color>( 4, Color::gray() ) );

	const auto torus = s2::GeometryFactory3D::createTorus( 1.0, 0.5, 64, 16 );
	_torus = RenderCore::VertexData::New( torus );
	_torus->setColors( std::vector<Color>( torus.vertices.size(), Color::red()) );

	const auto cube = s2::GeometryFactory3D::createCube( {5.0, 0.0, 0.0}, 2.0 );
	_cube = RenderCore::VertexData::New( cube );
	_cube->setColors( std::vector<Color>( cube.vertices.size(), Color::green()) );

	const auto cone = s2::GeometryFactory3D::createCone( Math::dvec3(2.5,0.0,0.0), Math::dvec3(2.5, 0.0, 3.0), 1, true, 32 );
	_cone = RenderCore::VertexData::New( cone );
	_cone->setColors( std::vector<Color>( cone.vertices.size(), Color::yellow()) );	

	const auto sphere = s2::GeometryFactory3D::createSphere( Math::dvec3( -2.5, 0.0, 0.0 ), 1.0, 32 );
	_sphere = RenderCore::VertexData::New( sphere );
	_sphere->setColors( std::vector<Color>( sphere.vertices.size(), Color::blue().lighter() ) );

	const auto cylinder = s2::GeometryFactory3D::createCylinder( Math::dvec3( -5.0, 0.0, 0.0 ), Math::dvec3( -5.0, 0.0, 2.0 ), 1.0, true, true, 32 );
	_cylinder = RenderCore::VertexData::New( cylinder );
	_cylinder->setColors( std::vector<Color>( cylinder.vertices.size(), Color::cyan()) );

	//const auto teapot = s2::GeometryFactory3D::createTeapot( 10,10 );
	//_teapot = RenderCore::VertexData::New();
	//_teapot->setVertices( vector_cast<Math::dvec3,Math::vec3>(teapot.vertices) );
	//_teapot->setNormals( vector_cast<Math::dvec3,Math::vec3>( teapot.normals ) );
	//_teapot->setIndices( teapot.indices );
	//_teapot->setColors( std::vector<Color>( teapot.vertices.size(), Color::orange()) );

			
	_camera.set( Math::dvec3( 0.0, 0.0, 8.0 ),
				 Math::dvec3( 0.0, 0.0, 0.0 ),
				 Math::dvec3( 0.0, 1.0, 0.0) );

	//_trackball.setRadius( 1.0 );
	_trackball.setCenter( Math::ivec2( width() / 2, height() / 2 ) );

	//_trackballLight.setRadius( 1.0 );
	_trackballLight.setCenter( Math::ivec2( width() / 2, height() / 2 ) );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onResizeEvent( uint32_t width, uint32_t height )
{
	const auto vp = Math::irect( 0, 0, width, height );
	if( vp.isEmpty() )
		return;

	_camera.setViewport( vp );
	_camera.setProjectionTransform( Math::ProjectionTransform::createPerspective( width / (double) height, 45.0, 0.1, 100.0 ) );

	_trackball.resize( width, height );
	_trackballLight.resize( width, height );
}


// ------------------------------------------------------------------------------------------------
void MainWindow::onCloseEvent()
{
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onPaintEvent()
{
	// Scene.draw
	auto app = static_cast<MyApplication*>( s2::Application::instance() );

	const auto scale = app->scaleFactor;
	const auto lightPosition = app->lightPosition;

	{
		using namespace RenderCore;

		_renderTarget->clear( { .color = Color{ 0.3f, 0.1f, 0.4f, 1.0f } } );

		DrawState ds( DefaultShaders.BlinnPhong );
		{
			ds.viewState.projectionMatrix = _camera.projectionMatrix();
			ds.viewState.modelMatrix      = Math::scale( Math::dvec3( scale ) ) *_trackball.matrix();
			ds.viewState.viewMatrix       = _camera.worldToCameraMatrix();
			ds.viewState.viewport         = _camera.viewport();

			ds.shader->setUniformValue<Math::mat4>( "projectionMatrix"         , ds.viewState.projectionMatrix );
			ds.shader->setUniformValue<Math::mat4>( "modelViewProjectionMatrix", ds.viewState.modelViewProjectionMatrix() );
			ds.shader->setUniformValue<Math::mat4>( "modelViewMatrix"          , ds.viewState.modelViewMatrix() );
			ds.shader->setUniformValue<Math::mat3>( "normalMatrix"             , ds.viewState.normalMatrix() );

			ds.shader->setUniformValue<Math::vec4>( "u_LightPosition"          , _trackballLight.matrix() * lightPosition );
			ds.shader->setUniformValue<Math::vec4>( "u_LightAmbient"           , { .1f,.1f,.1f,1.f } );
			ds.shader->setUniformValue<Math::vec4>( "u_LightDiffuse"           , { 1.f,1.f,1.f,1.f } );
			ds.shader->setUniformValue<Math::vec4>( "u_LightSpecular"          , { 1.f,1.f,1.f,1.f } );
			ds.shader->setUniformValue<float>(      "u_LightShininess"         , 160.f );

		}		
		
		_renderTarget->draw( PrimitiveType::Triangles, _torus,    ds );
		_renderTarget->draw( PrimitiveType::Triangles, _cone,     ds );
		_renderTarget->draw( PrimitiveType::Triangles, _cube,     ds );
		_renderTarget->draw( PrimitiveType::Triangles, _sphere,   ds );
		_renderTarget->draw( PrimitiveType::Triangles, _cylinder, ds );
		//_renderTarget->draw( PrimitiveType::Triangles, _teapot, ds );

		DrawState ds2 = ds;
		{
			ds2.viewState.modelMatrix = Math::dmat4(1.0);

			ds2.shader->setUniformValue<Math::mat4>( "modelViewProjectionMatrix", ds2.viewState.modelViewProjectionMatrix() );
			ds2.shader->setUniformValue<Math::mat4>( "modelViewMatrix"          , ds2.viewState.modelViewMatrix() );
			ds2.shader->setUniformValue<Math::mat3>( "normalMatrix"             , ds2.viewState.normalMatrix() );
		}

		//_renderTarget->draw( PrimitiveType::Triangles,
		//					 _planeXZ,
		//					  ds2 );
	}

	{
	}
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseMoveEvent( const s2::Input::MouseState& ms )
{
	// handle dragging of light trackball
	if( ms.isDragging() && ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )
		_trackballLight.update( Scene::TrackBall::DragEvent::Update, ms.position() );


	// handle dragging of object trackball
	if( ms.isDragging() && ms.isButtonDown( s2::Input::MouseState::ButtonLeft ) )
		_trackball.update( Scene::TrackBall::DragEvent::Update, ms.position() );

//	ms.dumpStatus( "onMouseMoveEvent" );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseDoubleClickEvent( const s2::Input::MouseState& ms )
{
//	ms.dumpStatus( "onMouseDoubleClickEvent" );

}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseButtonEvent( const s2::Input::MouseState& ms )
{
//	ms.dumpStatus( "onMouseButtonEvent" );

	if( ms.isButtonDown( s2::Input::MouseState::ButtonLeft) )		_trackball.update( Scene::TrackBall::DragEvent::Begin, ms.position() );
	else if( ms.isButtonUp( s2::Input::MouseState::ButtonLeft ) )   _trackball.update( Scene::TrackBall::DragEvent::End, ms.position() );



	if( ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )    _trackballLight.update( Scene::TrackBall::DragEvent::Begin, ms.position() );
	else if( ms.isButtonUp( s2::Input::MouseState::ButtonRight ) ) _trackballLight.update( Scene::TrackBall::DragEvent::End, ms.position() );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseScrollEvent( const s2::Input::MouseState& ms )
{
	//ms.dumpStatus( "onMouseScrollEvent" );
	auto app = static_cast<MyApplication*>( s2::Application::instance() );
	app->scaleFactor *= std::pow( 1.2, ms.wheel() );

	// Optionally add limits to prevent extreme scaling
	app->scaleFactor = std::max( 0.1, std::min( 10.0, app->scaleFactor ) );
}