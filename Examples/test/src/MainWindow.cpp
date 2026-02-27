// MainWindow.cpp
//
#include "MainWindow.h"

#include "MyApplication.h"

#include "Application/Application.h"

#include "RenderCore/RenderTarget.h"
#include "RenderCore/Context.h"
#include "RenderCore/RenderCommands.h"
#include "Renderer/RenderMaterial.h"

#include "Geometry/GeometryFactory3D.h"

#include <iostream>

////// TODO:
// - add keyboard handling
// - computeshaders and opengl 4.6


/* memo:
* 
* resourceManager::registerMesh accept meshdata instead of vertexdata, and create vertexdata internally.
* dynamic mesh
* static mesh
* 
* set uniform value bugged?
*
*
*/

// ------------------------------------------------------------------------------------------------
void MainWindow::loadResources()
{
	_texture = s2::Resources::ImageLoader::loadFromFile( "F:/Sviluppo/Projects/S2Engine/Examples/test/x64/Debug/assets/PNG/Light/texture_11.png" ).value_or( s2::Resources::ImageData {} );

	if( _texture.pixmap.isEmpty() )
	{
		std::cout << "Failed to load texture" << std::endl;
		return;
	}
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onInitializeEvent()
{
	loadResources();

	// Initialize renderer with the current rendering context and default render pipeline
	_renderer = std::make_unique<s2::Renderer::Renderer>( _renderingContext.get(), s2::Renderer::RenderPipeline::createForwardPipeline() );

	auto& resources = _renderer->resources();

	//// register torus mesh
	//{
	//	const auto mesh = s2::GeometryFactory3D::createTorus( 1.0, 0.5, 64, 16 );
	//	auto vtx = RenderCore::VertexData::New( mesh );
	//	vtx->setColors( std::vector<Color>( mesh.vertices.size(), Color::red() ) );
	//	_torus = resources.registerMesh( "torus", vtx );
	//}
	
	//// register cube mesh
	//{
	//	const auto mesh = s2::GeometryFactory3D::createCube( { 5.0, 0.0, 0.0 }, 2.0 );
	//	auto vtx = RenderCore::VertexData::New( mesh );
	//	vtx->setColors( std::vector<Color>( mesh.vertices.size(), Color::green() ) );
	//	_cube = resources.registerMesh( "cube", vtx );
	//}

	//// register cone mesh
	//{
	//	const auto mesh = s2::GeometryFactory3D::createCone( Math::dvec3(2.5,0.0,0.0), Math::dvec3(2.5, 0.0, 3.0), 1, true, 32 );
	//	auto vtx = RenderCore::VertexData::New( mesh );
	//	vtx->setColors( std::vector<Color>( mesh.vertices.size(), Color::yellow() ) );
	//	_cone = resources.registerMesh( "cone", vtx );
	//}

	//// register sphere mesh
	//{
	//	const auto mesh = s2::GeometryFactory3D::createSphere( Math::dvec3( -2.5, 0.0, 0.0 ), 1.0, 32 );
	//	auto vtx = RenderCore::VertexData::New( mesh );
	//	vtx->setColors( std::vector<Color>( mesh.vertices.size(), Color::blue().lighter() ) );
	//	_sphere = resources.registerMesh( "sphere", vtx );
	//}

	//// register cylinder mesh
	//{
	//	const auto mesh = s2::GeometryFactory3D::createCylinder( Math::dvec3( -5.0, 0.0, 0.0 ), Math::dvec3( -5.0, 0.0, 2.0 ), 1.0, true, true, 32 );
	//	auto vtx = RenderCore::VertexData::New( mesh );
	//	vtx->setColors( std::vector<Color>( mesh.vertices.size(), Color::cyan() ) );
	//	_cylinder = resources.registerMesh( "cylinder", vtx );
	//}

	//_material.shader = resources.registerShader( "blinnPhong", s2::RenderCore::DefaultShaders.Simple);
	//_material.textures["u_DiffuseMap"] = (int) resources.registerTexture( "orange",
	//														 s2::RenderCore::Texture2D::New(
	//														 s2::RenderCore::TextureDescription(
	//														 _texture.pixmap.width(),
	//														 _texture.pixmap.height(),
	//														 s2::RenderCore::TextureFormat::RedGreenBlue8 ),
	//														 (void*) _texture.pixmap.pixels() ) );
	//_material.properties["u_UseDiffuseMap"] = false;

	//const auto teapot = s2::GeometryFactory3D::createTeapot( 10,10 );
	//_teapot = RenderCore::VertexData::New();
	//_teapot->setVertices( vector_cast<Math::dvec3,Math::vec3>(teapot.vertices) );
	//_teapot->setNormals( vector_cast<Math::dvec3,Math::vec3>( teapot.normals ) );
	//_teapot->setIndices( teapot.indices );
	//_teapot->setColors( std::vector<Color>( teapot.vertices.size(), Color::orange()) );

	_camera.set( Math::dvec3( 0.0, 0.0, 8.0 ),
				 Math::dvec3( 0.0, 0.0, 0.0 ),
				 Math::dvec3( 0.0, 1.0, 0.0 )
	);

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

	using namespace s2::Renderer;

	// setup material properties and shader
	// note: no need to do this every frame if the material properties are static.
	// we can create a material instance once and reuse it for multiple draw calls and update it only when properties change.
	_material.properties["u_LightPosition"]  = Math::vec4(_trackballLight.matrix() * lightPosition);
	_material.properties["u_LightAmbient"]   = Math::vec4{ .01f,.01f,.01f,1.f };
	_material.properties["u_LightDiffuse"]   = Math::vec4{ 1.f,1.f,1.f,1.f };
	_material.properties["u_LightSpecular"]  = Math::vec4{ 1.f,1.f,1.f,1.f };
	_material.properties["u_LightShininess"] = 60.f;

	auto modelMatrix = Math::scale( Math::dvec3( scale ) ) * _trackball.matrix();

	_renderer->beginFrame( {
		.mainTarget             = _renderTarget.get(),
		.cameraViewMatrix       = _camera.worldToCameraMatrix(),
		.cameraProjectionMatrix = _camera.projectionMatrix(),
						   } );
	{
		_renderer->clear( { .color = Color{ 0.5f, float(scale), 0.3f, 1.0f } } );
			
		//_renderer->render(
		//	{
		//	.renderMode  = s2::Renderer::RenderMode::Triangles,
		//	.material    = _material,
		//	.mesh        = _sphere,
		//	.modelMatrix = modelMatrix,
		//	} );

		//_renderer->render(
		//	{
		//	.renderMode  = s2::Renderer::RenderMode::Triangles,
		//	.material    = _material,
		//	.mesh        = _torus,
		//	.modelMatrix = modelMatrix,
		//	} );

		//_renderer->render(
		//	{
		//	.renderMode  = s2::Renderer::RenderMode::Triangles,
		//	.material    = _material,
		//	.mesh        = _cone,
		//	.modelMatrix = modelMatrix,
		//	} );

		//{	
		//	s2::Renderer::RenderCommand cmd
		//	{
		//		.renderMode = s2::Renderer::RenderMode::Triangles,
		//		.material = _material,
		//		.mesh = _cube,
		//		.modelMatrix = modelMatrix,
		//	};
		//	cmd.material.properties["u_UseDiffuseMap"] = true;
		//	_renderer->render( cmd );
		//}

		//_renderer->render(
		//	{
		//	.renderMode  = s2::Renderer::RenderMode::Triangles,
		//	.material    = _material,
		//	.mesh        = _cylinder,
		//	.modelMatrix = modelMatrix,
		//	} );
	}
	_renderer->endFrame();
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