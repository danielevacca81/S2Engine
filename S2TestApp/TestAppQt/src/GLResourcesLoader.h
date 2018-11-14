#pragma once


#include "renderer/Context.h"
#include "renderer/Program.h"
#include "renderer/PrimitiveBuffer.h"

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
	static s2::Renderer::ContextPtr _mainContext;
	static s2::Renderer::PrimitiveBufferPtr       _torus;
	static s2::Renderer::PrimitiveBufferPtr       _cube;
	static s2::Renderer::ProgramPtr    _phong;
	static s2::Renderer::ProgramPtr    _background;
	static s2::Renderer::ProgramPtr    _simpleShader;
	//static int _composite;
};