// GLRenderWidget.cpp
//
#include "GLWidget.h"

#include <iostream>

using namespace s2::Qt;

// ------------------------------------------------------------------------------------------------
GLWidget::GLWidget( QWidget *parent )
: QOpenGLWidget( parent )
{
    makeCurrent();
}

// ------------------------------------------------------------------------------------------------
GLWidget::~GLWidget()
{}

// ------------------------------------------------------------------------------------------------
void GLWidget::initializeGL()
{
	QOpenGLWidget::initializeGL();
	emit openGLInitialized();
}