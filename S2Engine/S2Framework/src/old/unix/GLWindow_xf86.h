// GLWindow_xf86.h
//
#ifndef GLWINDOW_ONCE
#define GLWINDOW_ONCE

#include <X11/X.h>
#include <X11/keysym.h>

#include <string>

namespace s2Core {
class GLContext;

class GLWindow
{
private:
	Window win; // X window handler

	unsigned int	windowWidth;
	unsigned int	windowHeight;
	std::string     windowTitle;
	bool windowed;

    void create();
    void destroy();


public:
	GLWindow( const std::string &title = std::string("") );
	virtual ~GLWindow();

	bool processWinMessages();
	void show( bool visible = true );
	void hide();


	// OpenGL specific methods
	//virtual void	updateGL();
	//virtual void	paintGL();
	//virtual void	resizeGL();

    void setSize(unsigned int w, unsigned int h);
	void setWindowed( bool windowed );

	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getHWND();
};
}

#endif
