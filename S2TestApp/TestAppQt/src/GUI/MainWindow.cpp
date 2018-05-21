// MainWindow.cpp
// 
#include "MainWindow.h"

#include "S2Qt/GLGraphicsView.h"
#include "S2Qt/GLWidget.h"

#include "TestScene.h"

#include <QVBoxLayout>

// ------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
	_ui.setupUi(this);
	
	gui_createGLArea();
}

// ------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{}

// ------------------------------------------------------------------------------------------------
void MainWindow::gui_createGLArea()
{
	s2Qt::GLGraphicsView  *view  = new s2Qt::GLGraphicsView( this, true );
	_scene                       = new TestScene( view );
	view->setScene( _scene );

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget( view );

	_ui.frameGLArea->setLayout(layout);
}