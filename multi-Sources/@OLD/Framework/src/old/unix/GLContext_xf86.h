// GLContext_xf86.h
//
#ifndef GLCONTEXT_ONCE
#define GLCONTEXT_ONCE

#include <GL/glx.h>

namespace s2Core{

class GLContext
{
private:
    Window      window;
    GLXContext	ctx;

    void create();
    void destroy();
    void initExtensions();


public:
	GLContext( unsigned int hWnd );
	virtual ~GLContext();

    void makeCurrent();
    void swapBuffers();
};
}

#endif
