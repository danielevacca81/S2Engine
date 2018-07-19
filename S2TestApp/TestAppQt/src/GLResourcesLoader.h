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
	s2::OpenGL::Mesh       _torus;
	s2::OpenGL::Mesh       _cube;
	s2::OpenGL::ProgramPtr _phong;

};