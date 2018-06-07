// GLRenderWidget.cpp
//
#include "GLWidget.h"

#include <iostream>

using namespace s2Qt;


//QSurfaceFormat format;
//format.setDepthBufferSize(24);
//format.setStencilBufferSize(8);
//format.setVersion(3, 2);
//format.setProfile(QSurfaceFormat::CoreProfile);
//widget->setFormat(format); // must be called before the widget or its parent window gets shown

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
//static QGLWidget *gSharedGLWidget = Q_NULLPTR;
//static QGLWidget *getSharedGLWidget()
//{
//    if( !gSharedGLWidget )
//    {
//        QGLFormat f = QGLFormat::defaultFormat();
//		f.setDoubleBuffer( true );
//		f.setSampleBuffers( true );
//        f.setAlpha( true );
//        QGLFormat::setDefaultFormat( f );
//
//        gSharedGLWidget = new QGLWidget( f );
//    }
//    return gSharedGLWidget;
//}


// ------------------------------------------------------------------------------------------------
GLWidget::GLWidget( QWidget *parent )
: QOpenGLWidget( parent )//, getSharedGLWidget() )
{
    makeCurrent();
	auto  c = context();
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

// ------------------------------------------------------------------------------------------------
//void GLWidget::enableGestures()
//{
//    grabGesture( Qt::PanGesture );
//    grabGesture( Qt::PinchGesture );
//    grabGesture( Qt::SwipeGesture );
//}
