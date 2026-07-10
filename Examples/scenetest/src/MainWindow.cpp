// MainWindow.cpp
//
#include "MainWindow.h"

#include "MyApplication.h"

#include "Application/Application.h"

#include "RenderCore/ShaderCompiler.h"
#include "RenderCore/DefaultShaders.h"

#include "Renderer/ResourceManager.h"
#include "Renderer/RenderMaterial.h"
#include "Renderer/Renderer.h"
#include "Renderer/PickPass.h"
#include "Renderer/ForwardPass.h"

#include "Scene/RenderableComponent.h"


#include "Geometry/GeometryFactory3D.h"

#include <iostream>

using namespace s2;

// ------------------------------------------------------------------------------------------------
bool MainWindow::loadResources()
{
	#if 0
	//static std::filesystem::path assetBasePath( R"(E:\@Devel\Assets\Meterials)" );  
	static std::filesystem::path assetBasePath (R"(F:\Sviluppo\Materials\group14)" );  
	struct texturetag  
	{  
		std::string           name;
		std::filesystem::path path;
	};  
  
	std::array<texturetag, 5> texturesToLoad =   
	{  
		texturetag{ "pbr_albedo",    assetBasePath / R"(g5\g5_basecolor.png)" },  
		texturetag{ "pbr_normal",    assetBasePath / R"(g5\g5_normal.png)" },  
		texturetag{ "pbr_metallic",  assetBasePath / R"(g5\g5_metallic.png)" },  
		texturetag{ "pbr_roughness", assetBasePath / R"(g5\g5_roughness.png)" },  
		texturetag{ "pbr_ao",        assetBasePath / R"(g5\g5_ao.png)" }  
	};  
  
  
  
	auto& resourceManager = _renderer->resources();  
  
	for( const auto& tex : texturesToLoad )  
	{  
		if( auto handle = resourceManager.registerTexture( tex.name,  tex.path ); handle.isValid() )
			std::cout << "Loaded texture: " << tex.name << std::endl;
		else
		{
			std::cout << "Failed to load texture: " << tex.name << std::endl;
			return false;
		}
	}

#endif

	return true;
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onInitializeEvent()
{
	_renderPasses.emplace( "forward", std::make_shared<s2::Renderer::ForwardPass>() );
	//_renderPasses.emplace( "pick", std::make_shared<s2::Renderer::PickPass>() );
	//_renderPasses.emplace( "ui", std::make_shared<s2::Renderer::UIPass>() );

#if 0
	// Initialize the picker and connect to pick results
	_picker = std::make_unique<s2::Renderer::Picker>( *_renderer.get() );

	// capture 'this' so the lambda can update UI selection state
	_picker->onObjectHit( [this]( const s2::Renderer::PickResult& result )
	{
		if( result.isHit() )
		{
		}
		else
		{
		}
	} );
#endif

	if (!loadResources())
	{
		std::cerr << "Failed to load resources" << std::endl;
		return;
	}

	auto resources = &_renderer->resources();
	resources->registerShader( "blinnPhong", s2::RenderCore::DefaultShaders.BlinnPhong );
	resources->registerMesh( "cube", GeometryFactory3D::createCube( { 0, 0, 0 }, 1.0 ) );

	s2::Renderer::MaterialDefinition blinnPhongMaterialDef;
	blinnPhongMaterialDef.shader = resources->shader( "blinnPhong" );
	blinnPhongMaterialDef.state = {};

	auto cubeMat = blinnPhongMaterialDef.createMaterial();

	//_view.setViewMatrix( Math::lookAt( Math::dvec3( 0.0, 0.0, 8.0 ), Math::dvec3( 0.0, 0.0, 0.0 ), Math::dvec3( 0.0, 1.0, 0.0 ) ) );
	
	_cam.setPerspective(  45.0 );
	_cam.setViewportSize( { width(), height() } );
	_cam.setPosition( { 0.0, 0.0, 8.0 } );


	_mainScene.setActiveCamera( _cam );

	

	// auto cube = _world.createEntity( "cubeEntity" )->addComponent<s2::Scene::RenderableComponent>();
	// cube->setMesh( resources->mesh( "cube" ) );
	// cube->setMaterialProperty( "BaseColor", Color{ 1.0f, 0.5f, 0.31f } );


}

// ------------------------------------------------------------------------------------------------
void MainWindow::onShutdownEvent()
{
	// free render passes
	_renderPasses.clear();

//	_picker.reset();
//	_thumbnailTarget.reset();
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onResizeEvent( uint32_t width, uint32_t height )
{
	const auto vp = Math::irect( 0, 0, width, height );
	if( vp.isEmpty() )
		return;

	_view.setProjectionTransform(  Math::ProjectionTransform::createPerspective( width / (double) height, 45.0, 0.1, 100.0 ) );
	_view.setViewport( vp );
}


// ------------------------------------------------------------------------------------------------
void MainWindow::onCloseEvent()
{
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onDraw()
{
	// Scene.draw
	auto app = static_cast<MyApplication*>( s2::Application::instance() );

	const auto scale = app->scaleFactor;

	auto& resources = _renderer->resources();

	using namespace s2::Renderer;

	// _renderer->begin(
	// 	{
	// 		.renderPasses = { _renderPasses["forward"] },
	// 		.renderTarget = _mainRenderTarget.get(),
	// 		.view         = _view,
	// 	} );

	// _renderer->submit( { .clearColor = Color{ 0.3f, 0.4f, 0.5f, 1.0f } } );
	// _renderer->execute();
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseMoveEvent( const s2::Input::MouseState& ms )
{
	if( uiWantCaptureMouse() )
		return;

}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseDoubleClickEvent( const s2::Input::MouseState& ms )
{
	//	ms.dumpStatus( "onMouseDoubleClickEvent" );
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseButtonEvent( const s2::Input::MouseState& ms )
{
	if( uiWantCaptureMouse() )
		return;
}

// ------------------------------------------------------------------------------------------------
void MainWindow::onMouseScrollEvent( const s2::Input::MouseState& ms )
{
	if( uiWantCaptureMouse() )
		return;

}