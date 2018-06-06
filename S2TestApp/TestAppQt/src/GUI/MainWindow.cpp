// MainWindow.cpp
// 
#include "MainWindow.h"

#include "qtbridge/GLGraphicsView.h"
#include "qtbridge/GLWidget.h"

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
	_scene          = new TestScene(this);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget( new QtBridge::GLGraphicsView( _scene, this, QFrame::Shape::NoFrame ) );

	_ui.frameGLArea->setLayout(layout);

	//_scene->postInitialization();
}