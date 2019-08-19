// GLContext.cpp
//
#include "GLContext.h"

#include <QOpenGLContext>
#include <QApplication>

#include <iostream>

// ------------------------------------------------------------------------------------------------
bool s2::Qt::createOpenGLContext( int openGLMajorVer, int openGLMinorVer )
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
			      << " not supported" << std::endl;
		return false;
	}

	std::cout << "OpenGL Version " << context.format().majorVersion() << "." << context.format().minorVersion() << std::endl;
	std::cout << "    Doublebuffer : " << ( context.format().swapBehavior() == QSurfaceFormat::DoubleBuffer ? "true" : "false" ) << std::endl;
	std::cout << "    Stencilbuffer: " << context.format().stencilBufferSize() << std::endl;
	std::cout << "    SampleBuffers: " << context.format().samples() << std::endl;
	std::cout << std::endl;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool s2::Qt::openGLSupports( int major, int minor, bool gles, QSurfaceFormat::OpenGLContextProfile profile )
{
    QOpenGLContext ctx;
    QSurfaceFormat fmt;
    fmt.setVersion(major, minor);
    if (gles) {
        fmt.setRenderableType(QSurfaceFormat::OpenGLES);
    } else {
        fmt.setRenderableType(QSurfaceFormat::OpenGL);
        fmt.setProfile(profile);
    }

    ctx.setFormat(fmt);
    ctx.create();
    if (!ctx.isValid())
        return false;
    int ctxMajor = ctx.format().majorVersion();
    int ctxMinor = ctx.format().minorVersion();
    bool isGles = (ctx.format().renderableType() == QSurfaceFormat::OpenGLES);

    if (isGles != gles) return false;
    if (ctxMajor < major) return false;
    if (ctxMajor == major && ctxMinor < minor)
        return false;
    if (!gles && ctx.format().profile() != profile)
        return false;
    return true;
}
