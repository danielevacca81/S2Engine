// MainWindow.cpp
// 
#include "MainWindow.h"

#include "TestScene.h"

#include "qt/GLGraphicsView.h"
#include "qt/GLWidget.h"

#include "math/Mesh.h"
#include "math/Geometry.h"

#include "opengl/Program.h"
#include "utils/String.h"


#include <QVBoxLayout>
#include <iostream>

// ------------------------------------------------------------------------------------------------
MainWindow::MainWindow( QWidget *parent )
: QMainWindow( parent )
, _res( this )
{
	_ui.setupUi( this );

	createScenes();

	connect( &_res, &GLResourcesLoader::resourcesInitialized, this, &MainWindow::initResources );
}

// ------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{}

// ------------------------------------------------------------------------------------------------
void MainWindow::createScenes()
{
	//if( false )
	{
		_sceneLeft          = new TestScene( this );

		//QVBoxLayout *layout = new QVBoxLayout;
		//layout->setContentsMargins( 0, 0, 0, 0 );
		//layout->addWidget( new s2::Qt::GLGraphicsView( _sceneLeft, this, QFrame::Shape::NoFrame ) );

		//_ui.frameGLAreaLeft->setLayout( layout );

		QVBoxLayout *layout = new QVBoxLayout;
		layout->setContentsMargins( 0, 0, 0, 0 );
		layout->addWidget( _sceneLeft );

		_ui.frameGLAreaLeft->setLayout( layout );
	}

	//if( false )
	{
		_sceneRight          = new TestScene( this );

		//QVBoxLayout *layout = new QVBoxLayout;
		//layout->setContentsMargins( 0, 0, 0, 0 );
		//layout->addWidget( new s2::Qt::GLGraphicsView( _sceneRight, this, QFrame::Shape::NoFrame ) );

		//_ui.frameGLAreaRight->setLayout( layout );

		QVBoxLayout *layout = new QVBoxLayout;
		layout->setContentsMargins( 0, 0, 0, 0 );
		layout->addWidget( _sceneRight );

		_ui.frameGLAreaRight->setLayout( layout );
	}
}

// ------------------------------------------------------------------------------------------------
void MainWindow::initResources()
{
	if( _sceneLeft )
	{
		_sceneLeft->addMesh( _res._cube );
		_sceneLeft->setShader( _res._phong );
	}

	if( _sceneRight )
	{
		_sceneRight->addMesh( _res._torus );
		_sceneRight->setShader( _res._phong );
	}
}

