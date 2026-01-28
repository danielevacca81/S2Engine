// MainWindow.cpp
//
#include "MainWindow.h"

#include "MyApplication.h"

#include "Application/Application.h"

#include "RenderCore/RenderTarget.h"
#include "RenderCore/RenderCore.h"

#include "Geometry/GeometryFactory3D.h"

////// TODO:
// - add keyboard handling
// - computeshaders and opengl 4.6


// ------------------------------------------------------------------------------------------------
template<class T, class U> requires std::is_convertible_v<T, U>
static inline std::vector<U> vector_cast( const std::vector<T>& v )
{
	std::vector<U> result;
	result.reserve( v.size() );
	for( const auto& e : v )
		result.push_back( static_cast<U>( e ) );
	return result;
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onInitializeEvent()
{
	_planeXZ = RenderCore::PrimitiveBuffer::New();
	_planeXZ->setVertices( std::vector<Math::vec3> {
		    { -2.5f, -2.5f, 0.f },
			{  2.5f, -2.5f, 0.f },
			{  2.5f,  2.5f, 0.f },
			{ -2.5f,  2.5f, 0.f }
	} );
	_planeXZ->setNormals( std::vector<Math::vec3>( 4, { 0.f, 0.f, 1.f } ) );
	_planeXZ->setIndices( { 0, 1, 2, 0, 2, 3 } );
	_planeXZ->setColors( std::vector<Color>( 4, Color::gray() ) );

	const auto geometry = Geometry::GeometryFactory3D::createTorus( 1.5, 0.65, 64, 16 );
	_torus = RenderCore::PrimitiveBuffer::New();
	_torus->setVertices( vector_cast<Math::dvec3,Math::vec3>(geometry.vertices) );
	_torus->setNormals( vector_cast<Math::dvec3,Math::vec3>( geometry.normals ) );
	_torus->setIndices( geometry.indices );
	_torus->setColors( std::vector<Color>( geometry.vertices.size(), Color::random()) );

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

		_renderTarget->clear( { .color = Color{ 0.2f, 0.4f, 0.6f, 1.0f } } );

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
		_renderTarget->draw( PrimitiveType::Triangles,
							 _torus,
							  ds );

		DrawState ds2 = ds;
		{
			ds2.viewState.modelMatrix = Math::dmat4(1.0);

			ds2.shader->setUniformValue<Math::mat4>( "modelViewProjectionMatrix", ds2.viewState.modelViewProjectionMatrix() );
			ds2.shader->setUniformValue<Math::mat4>( "modelViewMatrix"          , ds2.viewState.modelViewMatrix() );
			ds2.shader->setUniformValue<Math::mat3>( "normalMatrix"             , ds2.viewState.normalMatrix() );
		}

		_renderTarget->draw( PrimitiveType::Triangles,
							 _planeXZ,
							  ds2 );
	}

	{
	}
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseMoveEvent( const s2::Input::MouseState& ms )
{
	// handle dragging of light trackball
	if( ms.isDragging() && ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )
		_trackballLight.update( Graphics::TrackBall::DragEvent::Update, ms.position() );


	// handle dragging of object trackball
	if( ms.isDragging() && ms.isButtonDown( s2::Input::MouseState::ButtonLeft ) )
		_trackball.update( Graphics::TrackBall::DragEvent::Update, ms.position() );

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

	if( ms.isButtonDown( s2::Input::MouseState::ButtonLeft) )		_trackball.update( Graphics::TrackBall::DragEvent::Begin, ms.position() );
	else if( ms.isButtonUp( s2::Input::MouseState::ButtonLeft ) )   _trackball.update( Graphics::TrackBall::DragEvent::End, ms.position() );



	if( ms.isButtonDown( s2::Input::MouseState::ButtonRight ) )    _trackballLight.update( Graphics::TrackBall::DragEvent::Begin, ms.position() );
	else if( ms.isButtonUp( s2::Input::MouseState::ButtonRight ) ) _trackballLight.update( Graphics::TrackBall::DragEvent::End, ms.position() );
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