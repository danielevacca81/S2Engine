// MainWindow.h
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Application/Window.h"
#include "Application/MouseState.h"

#include "Geometry/MeshData.h"
#include "Resources/ImageLoader.h"

#include "Scene/Camera.h"
#include "Scene/TrackBall.h"

#include "Renderer/Renderer.h"

class MainWindow : public s2::Window
{
public:
	MainWindow( const std::string &name, int32_t width, int32_t height, const s2::WindowParameters &params )
		: s2::Window( name, width, height, params )
	{}
	

	void onInitializeEvent() override;
	void onPaintEvent() override;
	void onCloseEvent() override;

	void onMouseMoveEvent( const s2::Input::MouseState& ms ) override;
	void onMouseDoubleClickEvent( const s2::Input::MouseState& ms ) override;
	void onMouseButtonEvent( const s2::Input::MouseState& ms ) override;
	void onMouseScrollEvent( const s2::Input::MouseState& ms ) override;

	void onResizeEvent( uint32_t width, uint32_t height ) override;

private:
	void loadResources();

private:
	// s2::Renderer renderer; // forward rendering, deferred rendering, etc.
	std::unique_ptr<s2::Renderer::Renderer> _renderer;

	s2::Renderer::ResourceHandle _torus    { s2::Renderer::InvalidHandle };
	s2::Renderer::ResourceHandle _cone     { s2::Renderer::InvalidHandle };
	s2::Renderer::ResourceHandle _cube     { s2::Renderer::InvalidHandle };
	s2::Renderer::ResourceHandle _sphere   { s2::Renderer::InvalidHandle };
	s2::Renderer::ResourceHandle _cylinder { s2::Renderer::InvalidHandle };
	s2::Renderer::ResourceHandle _teapot   { s2::Renderer::InvalidHandle };

	s2::Renderer::RenderMaterial _material;
	s2::Resources::ImageData _texture;

	s2::Scene::Camera    _camera;
	s2::Scene::TrackBall _trackball;
	s2::Scene::TrackBall _trackballLight;
};

#endif // !MAINWINDOW_H
