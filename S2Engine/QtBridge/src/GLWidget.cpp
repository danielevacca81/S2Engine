// GLRenderWidget.cpp
//
#include "GLWidget.h"

#include <iostream>

using namespace QtBridge;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
static QGLWidget *gSharedGLWidget = Q_NULLPTR;
static QGLWidget *getSharedGLWidget()
{
    if( !gSharedGLWidget )
    {
        QGLFormat f = QGLFormat::defaultFormat();
		f.setDoubleBuffer( true );
		f.setSampleBuffers( true );
        f.setAlpha( true );
        QGLFormat::setDefaultFormat( f );

        gSharedGLWidget = new QGLWidget( f );
    }
    return gSharedGLWidget;
}


// ------------------------------------------------------------------------------------------------
GLWidget::GLWidget( QWidget *parent )
: QGLWidget( parent, getSharedGLWidget() )
{
    makeCurrent();

	std::wcout << "OpenGL Version "    << format().majorVersion() << "." << format().minorVersion() << std::endl;
	std::wcout << "   Doublebuffer: "  << format().doubleBuffer() << std::endl;
	std::wcout << "   Stencilbuffer: " << format().stencil() << std::endl;
	std::wcout << "   SampleBuffers: " << format().sampleBuffers() << std::endl;
	std::wcout << std::endl;
}

// ------------------------------------------------------------------------------------------------
GLWidget::~GLWidget()
{}

// ------------------------------------------------------------------------------------------------
void GLWidget::initializeGL()
{
    //OpenGL::glDumpContextInfo();
}

// ------------------------------------------------------------------------------------------------
//void GLWidget::enableGestures()
//{
//    grabGesture( Qt::PanGesture );
//    grabGesture( Qt::PinchGesture );
//    grabGesture( Qt::SwipeGesture );
//}
