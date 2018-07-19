// GLContext.h
//
#ifndef GUITOOLS_GLCONTEXT_H
#define GUITOOLS_GLCONTEXT_H

#include "s2Qt_API.h"

#include <QSurfaceFormat>

namespace s2 {

namespace Qt {

bool S2QT_API createOpenGLContext( int openGLMajorVer = 2, int openGLMinorVer = 1 );
bool S2QT_API openGLSupports( int major, int minor, bool gles = false, QSurfaceFormat::OpenGLContextProfile profile = QSurfaceFormat::NoProfile );

}
}

#endif
