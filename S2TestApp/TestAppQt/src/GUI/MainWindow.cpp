// MainWindow.cpp
// 
#include "MainWindow.h"

#include "qt/GLGraphicsView.h"
#include "qt/GLWidget.h"

#include "TestScene.h"

#include <QVBoxLayout>

using namespace s2;

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
	layout->addWidget( new s2::Qt::GLGraphicsView( _scene, this, QFrame::Shape::NoFrame ) );

	_ui.frameGLArea->setLayout(layout);

	//_scene->postInitialization();
}