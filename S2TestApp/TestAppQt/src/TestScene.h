// TestScene.h
//
#pragma once

#include "qt/GLWidget.h"

#include "renderer/PrimitiveBuffer.h"
#include "renderer/Program.h"
#include "renderer/FrameBuffer.h"
#include "renderer/Surface.h"
//#include "Renderer/TextRenderer.h"

#include "view/Trackball.h"
#include "view/Camera.h"
#include "view/ViewState.h"

#include <QOpenGLFunctions>

class TestScene: public s2::Qt::GLWidget
{
public:
	TestScene( QWidget *parent );
	~TestScene();

	void resetView();
	void toggleWireframe();

	void resizeGL( int w, int h ) override;
	void paintGL() override;
	void initializeGL() override;

	void updateAnimations() override;

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

	QTimer _t;

	bool              _wireframe;

	s2::SceneGraph::TrackBall     _trackball;
	s2::SceneGraph::Camera        _camera;

	s2::Math::dvec3 _center;
	//Math::dvec3 _target;
	//Math::dvec3 _up;

	s2::SceneGraph::ViewState  _viewState;
	//s2::OpenGL::FrameBufferPtr _fb;
	s2::Renderer::SurfacePtr _surface;
	//s2::OpenGL::ProgramPtr _shader;

	std::vector<s2::Renderer::PrimitiveBufferPtr> _meshes;
};