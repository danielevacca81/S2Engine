// MainWindow.h
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Application/Window.h"
#include "Application/MouseState.h"

#include "Scene/Scene.h"
#include "Renderer/View.h"
#include "Renderer/RenderPass.h"

#include <string>
#include <unordered_map>
#include <memory>

class MainWindow : public s2::Window
{
public:
	MainWindow( const std::string &name, int32_t width, int32_t height, const s2::WindowParameters &params )
		: s2::Window( name, width, height, params )
	{}
	

	void onInitializeEvent() override;
	void onShutdownEvent() override;
	void onCloseEvent() override;
	void onDraw() override;

	void onMouseMoveEvent( const s2::Input::MouseState& ms ) override;
	void onMouseDoubleClickEvent( const s2::Input::MouseState& ms ) override;
	void onMouseButtonEvent( const s2::Input::MouseState& ms ) override;
	void onMouseScrollEvent( const s2::Input::MouseState& ms ) override;

	void onResizeEvent( uint32_t width, uint32_t height ) override;

private:
	bool loadResources();

private:
	s2::Renderer::RenderPasses _renderPasses;

	//s2::Renderer::View _view;
	s2::Scene::Scene   _mainScene;
	s2::Scene::Camera  _cam;

};

#endif // !MAINWINDOW_H
