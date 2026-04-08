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
#include "Renderer/Picker.h"

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
	std::unique_ptr<s2::Renderer::Renderer> _renderer; // forward rendering, deferred rendering, etc.
	std::unique_ptr<s2::Renderer::Picker>   _picker;   // optional GPU picking utility

	s2::Renderer::ResourceHandle _cone     { s2::Renderer::InvalidHandle };

	s2::Renderer::RenderMaterial _material;
	s2::Renderer::RenderMaterial _materialPBR;

	s2::Scene::Camera    _camera;
	s2::Scene::TrackBall _trackball;
	s2::Scene::TrackBall _trackballLight;
};

#endif // !MAINWINDOW_H
