// TestScene.h
//
#pragma once

#include "qt/GLGraphicsScene.h"

#include "opengl/Mesh.h"
#include "opengl/Program.h"
#include "opengl/FrameBuffer.h"
//#include "Renderer/TextRenderer.h"

#include "view/Trackball.h"
#include "view/Camera.h"


#if 0
#include <QOpenGLWidget>
class TestScene : QOpenGLWidget
{
public:
	TestScene( QWidget *parent );
	~TestScene();

	void addMesh( const s2::OpenGL::Mesh &m );
	void setShader( const s2::OpenGL::ProgramPtr &shader );

	void resetView();
	void toggleWireframe();

	void resizeGL( int w, int h ) override;
	void paintGL() override;
	//void refreshScene() override;
	void initializeGL() override;


private:
	void onMousePressed();
	void onMouseReleased();
	void onMouseMoved();
	void onMouseWheel();
	void onMouseDoubleClick();

	void initFonts();

private slots:


private:
	bool              _wireframe;

	s2::Renderer::TrackBall     _trackball;
	s2::Renderer::Camera        _camera;

	s2::Math::dvec3 _center;
	//Math::dvec3 _target;
	//Math::dvec3 _up;

	s2::OpenGL::ViewState      _viewState;
	s2::OpenGL::FrameBufferPtr _fb;
	s2::OpenGL::ProgramPtr     _shader;

	std::vector<s2::OpenGL::Mesh> _meshes;
};

#else
class TestScene : public s2::Qt::GLGraphicsScene
{
public:
	TestScene( QWidget *parent );
	~TestScene();

	void addMesh( const s2::OpenGL::Mesh &m );
	void setShader( const s2::OpenGL::ProgramPtr &shader );

	void resetView();
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

	void initFonts();

private slots:
	void initializeOpenGL() override;


private:
	bool              _wireframe;

	s2::Renderer::TrackBall     _trackball;
	s2::Renderer::Camera        _camera;

	s2::Math::dvec3 _center;
	//Math::dvec3 _target;
	//Math::dvec3 _up;

	s2::OpenGL::ViewState      _viewState;
	s2::OpenGL::FrameBufferPtr _fb;
	s2::OpenGL::ProgramPtr     _shader;

	std::vector<s2::OpenGL::Mesh> _meshes;
};
#endif