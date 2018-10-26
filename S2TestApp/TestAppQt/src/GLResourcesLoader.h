#pragma once


#include "opengl/Context.h"
#include "opengl/Program.h"
#include "opengl/Mesh.h"

#include <QOffscreenSurface>
#include <QOpenGLWidget>

class GLResourcesLoader: public QOpenGLWidget
{
	Q_OBJECT

public:
	GLResourcesLoader( QWidget *parent );
	~GLResourcesLoader();

	bool initSharedResources();

	bool initMyVR();


private:
	void initializeGL() override;
	void initMeshes();
	void initShaders();

signals:
	void resourcesInitialized();

public:
	static s2::OpenGL::ContextPtr _mainContext;
	static s2::OpenGL::MeshPtr       _torus;
	static s2::OpenGL::MeshPtr       _cube;
	static s2::OpenGL::ProgramPtr    _phong;
	static s2::OpenGL::ProgramPtr    _background;
	//static int _composite;
};