// TestScene.h
//
#pragma once

#include "qt/GLGraphicsScene.h"

#include "opengl/Renderer.h"
#include "opengl/Mesh.h"
#include "opengl/Program.h"
//#include "Renderer/TextRenderer.h"

//#include "View/Trackball.h"
//#include "View/Camera.h"


class TestScene : public s2::Qt::GLGraphicsScene
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

	s2::OpenGL::Renderer  _renderer;
	s2::OpenGL::ViewState _viewState;
	
	//s2::TrackBall     _trackball;
	//s2::TextRenderer  _textRenderer;
	//s2::Camera        _camera;

	s2::Math::dvec3 _center;
	//Math::dvec3 _target;
	//Math::dvec3 _up;

	//OpenGL::DrawState  _drawState;
	//OpenGL::Context    _context;
	s2::OpenGL::ProgramPtr _shaderBlinnPhong;
	s2::OpenGL::ProgramPtr _shaderSimple;
	s2::OpenGL::Mesh       _cubeMesh;
	s2::OpenGL::Mesh       _triangle;
};