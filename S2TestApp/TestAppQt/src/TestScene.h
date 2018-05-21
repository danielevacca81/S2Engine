// TestScene.h
//
#pragma once

#include "S2Qt/GLGraphicsScene.h"

#include "VObjects/VObjectManager.h"

#include "Renderer/Renderer.h"
#include "OpenGL/VertexArray.h"
#include "Renderer/TextRenderer.h"

#include "View/Trackball.h"
#include "View/Camera.h"
#include "OpenGL/Context.h"
#include "OpenGL/Program.h"


class TestScene : public s2Qt::GLGraphicsScene
{
private:


	OpenGL::Renderer  _renderer;
	OpenGL::ViewState _viewState;

	s2::VObjectManager _objectManager;
	
	s2::TrackBall     _trackball;
	s2::TextRenderer  _textRenderer;
	s2::Camera        _camera;

	Math::dvec3 _center;
	//Math::dvec3 _target;
	//Math::dvec3 _up;

	//OpenGL::DrawState  _drawState;
	OpenGL::Context    _context;
	OpenGL::ProgramPtr _shader;
	OpenGL::Mesh        _cubeMesh;

	bool _wireframe;

	void initContext();
	void initShaders();
	void initFonts();

	void createMeshes();
	void createObjects();

public:
	TestScene( QWidget *parent );
	~TestScene();

	void postInitialization();

	//CadEngine *cadEngine() { return _cadEngine; }

	//void rotateScene( double degrees );
	//void zoomScene();
	//void translateScene();
	//void confirm();
	void toggleWireframe();

	void drawBackground( QPainter *painter, const QRectF &rect );

	void resizeScene( int w, int h );

	void onMousePressed();
	void onMouseReleased();
	void onMouseMoved();
	void onMouseWheel();
	void onMouseDoubleClick();
};