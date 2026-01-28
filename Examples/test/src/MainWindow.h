// MainWindow.h
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Application/Window.h"
#include "Application/MouseState.h"

#include "RenderCore/PrimitiveBuffer.h"
#include "Renderer/Camera.h"
#include "Renderer/TrackBall.h"

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
	RenderCore::PrimitiveBufferPtr _planeXZ;
	RenderCore::PrimitiveBufferPtr _torus;
	Graphics::Camera               _camera;
	Graphics::TrackBall            _trackball;
	Graphics::TrackBall            _trackballLight;
};

#endif // !MAINWINDOW_H
