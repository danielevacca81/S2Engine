#pragma once


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

	bool load();


private:
	void initializeGL() override;
	void initMeshes();
	void initShaders();

signals:
	void resourcesInitialized();

public:
	static s2::OpenGL::ContextPtr _mainContext;
	static s2::OpenGL::MeshPtr    _torus;
	static s2::OpenGL::MeshPtr    _cube;
	static s2::OpenGL::ProgramPtr _phong;
	static s2::OpenGL::ProgramPtr _background;
};