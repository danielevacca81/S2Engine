#pragma once

#include <QOpenGLWidget>

#include "opengl/Program.h"
#include "opengl/Mesh.h"


class GLResourcesLoader: public QOpenGLWidget
{
	Q_OBJECT

public:
	GLResourcesLoader( QWidget *parent );
	~GLResourcesLoader();

	bool load();


private:
	void initializeGL() override;
	void resizeGL( int width, int height ) override;
	void paintGL()override;
	void initMeshes();
	void initShaders();

signals:
	void resourcesInitialized();

public:
	s2::OpenGL::Mesh       _torus;
	s2::OpenGL::Mesh       _cube;
	s2::OpenGL::ProgramPtr _phong;

};