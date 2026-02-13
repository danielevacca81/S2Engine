// MainWindow.h
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Application/Window.h"
#include "Application/MouseState.h"

#include "RenderCore/VertexData.h" // deprecated, use MeshData instead

#include "Geometry/MeshData.h"


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
	void onPaintEventold() ;
	void onCloseEvent() override;

	void onMouseMoveEvent( const s2::Input::MouseState& ms ) override;
	void onMouseDoubleClickEvent( const s2::Input::MouseState& ms ) override;
	void onMouseButtonEvent( const s2::Input::MouseState& ms ) override;
	void onMouseScrollEvent( const s2::Input::MouseState& ms ) override;

	void onResizeEvent( uint32_t width, uint32_t height ) override;

private:
	// s2::Renderer renderer; // forward rendering, deferred rendering, etc.
	std::unique_ptr<s2::Renderer::Renderer> _renderer;

	s2::MeshData3D _torus;

	s2::RenderCore::VertexDataPtr _planeXZ;
	s2::RenderCore::VertexDataPtr _cone;
	s2::RenderCore::VertexDataPtr _cube;
	s2::RenderCore::VertexDataPtr _sphere;
	s2::RenderCore::VertexDataPtr _cylinder;
	s2::RenderCore::VertexDataPtr _teapot;


	s2::Scene::Camera               _camera;
	s2::Scene::TrackBall            _trackball;
	s2::Scene::TrackBall            _trackballLight;
};

#endif // !MAINWINDOW_H
