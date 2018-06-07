// GLContext.cpp
//
#include "GLContext.h"

#include <QOpenGLContext>
#include <QApplication>

#include <iostream>

// ------------------------------------------------------------------------------------------------
bool s2Qt::createOpenGLContext( int openGLMajorVer, int openGLMinorVer )
{
	QSurfaceFormat f = QSurfaceFormat::defaultFormat();
    f.setVersion( openGLMajorVer, openGLMinorVer );
    f.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
    f.setDepthBufferSize( 24 );
    f.setAlphaBufferSize( 8 );
    f.setStencilBufferSize( 8 );
    f.setSamples( 4 );

    if( openGLMajorVer <= 2 ) f.setProfile( QSurfaceFormat::NoProfile );
    else                      f.setProfile( QSurfaceFormat::CompatibilityProfile );

    QSurfaceFormat::setDefaultFormat( f );

	QOpenGLContext context;// = QOpenGLContext::globalShareContext();
	if( !context.create() )
	{
		std::cout << "OpenGL Version " << context.format().majorVersion() << "." << context.format().minorVersion() 
			      << " not supported";
		return false;
	}

	std::cout << "OpenGL Version " << context.format().majorVersion() << "." << context.format().minorVersion() << std::endl;
	std::cout << "    Doublebuffer : " << ( context.format().swapBehavior() == QSurfaceFormat::DoubleBuffer ? "true" : "false" ) << std::endl;
	std::cout << "    Stencilbuffer: " << context.format().stencilBufferSize() << std::endl;
	std::cout << "    SampleBuffers: " << context.format().samples() << std::endl;
	std::cout << std::endl;
	return true;
}