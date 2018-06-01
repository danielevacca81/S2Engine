// GLContext.cpp
//
#include "GLContext.h"

#include <QOpenGLContext>
#include <QApplication>

// ------------------------------------------------------------------------------------------------
bool QtBridge::createOpenGLContext( int openGLMajorVer, int openGLMinorVer, bool shareOpenGLContexts )
{
    QSurfaceFormat f = QSurfaceFormat::defaultFormat();
    f.setVersion( openGLMajorVer, openGLMinorVer );
    f.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
    f.setDepthBufferSize( 24 );
    f.setAlphaBufferSize( 8 );
    //f.setStencilBufferSize( 8 );
    //f.setSamples( 4 );

    if( openGLMajorVer <= 2 ) f.setProfile( QSurfaceFormat::NoProfile );
    else                      f.setProfile( QSurfaceFormat::CompatibilityProfile );

    QSurfaceFormat::setDefaultFormat( f );

    bool ok = true;
    if( shareOpenGLContexts )
        QApplication::setAttribute( Qt::AA_ShareOpenGLContexts );

    // @todo check if supported
    return ok;
}