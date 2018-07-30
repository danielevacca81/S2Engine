// TestScene.h
//
#pragma once

#include "qt/GLWidget.h"

#include "opengl/Mesh.h"
#include "opengl/Program.h"
#include "opengl/FrameBuffer.h"
#include "opengl/Surface.h"
//#include "Renderer/TextRenderer.h"

#include "view/Trackball.h"
#include "view/Camera.h"

#include <QOpenGLFunctions>

class TestScene: public s2::Qt::GLWidget
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
	void initializeGL() override;

private:
	void onMousePressed();
	void onMouseReleased();
	void onMouseMoved();
	void onMouseWheel();
	void onMouseDoubleClick();

	void initFonts();

private:
    int   _frames;
    QTime _time;


	bool              _wireframe;

	s2::Renderer::TrackBall     _trackball;
	s2::Renderer::Camera        _camera;

	s2::Math::dvec3 _center;
	//Math::dvec3 _target;
	//Math::dvec3 _up;

	s2::OpenGL::ViewState      _viewState;
	//s2::OpenGL::FrameBufferPtr _fb;
	s2::OpenGL::SurfacePtr _surface;
	s2::OpenGL::ProgramPtr     _shader;

	std::vector<s2::OpenGL::Mesh> _meshes;
};