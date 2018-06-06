// TestScene.h
//
#pragma once

#include "qtbridge/GLGraphicsScene.h"

#include "Renderer/Renderer.h"
#include "OpenGL/VertexArray.h"
#include "Renderer/TextRenderer.h"

#include "View/Trackball.h"
#include "View/Camera.h"
#include "OpenGL/Context.h"
#include "OpenGL/Program.h"


class TestScene : public QtBridge::GLGraphicsScene
{
public:
	TestScene( QWidget *parent );
	~TestScene();

	void postInitialization();

	void toggleWireframe();

	void resizeScene( int w, int h ) override;
	void renderScene() override;
	void refreshScene() override;


private:
	void onMousePressed();
	void onMouseReleased();
	void onMouseMoved();
	void onMouseWheel();
	void onMouseDoubleClick();

	//void initContext();
	void initShaders();
	void initFonts();

	void createMeshes();
	void createObjects();

private slots:
	void initializeOpenGL() override;


private:
	bool              _wireframe;

	OpenGL::Renderer  _renderer;
	OpenGL::ViewState _viewState;
	
	s2::TrackBall     _trackball;
	s2::TextRenderer  _textRenderer;
	s2::Camera        _camera;

	Math::dvec3 _center;
	//Math::dvec3 _target;
	//Math::dvec3 _up;

	//OpenGL::DrawState  _drawState;
	//OpenGL::Context    _context;
	OpenGL::ProgramPtr _shaderBlinnPhong;
	OpenGL::ProgramPtr _shaderSimple;
	OpenGL::Mesh       _cubeMesh;
	OpenGL::Mesh       _triangle;
};