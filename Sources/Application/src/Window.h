// Window.h
//
#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include "Application_API.h"

#include "WindowParameters.h"

#include <string>
#include <vector>

namespace RenderSystem {
class Context;
}

namespace s2 {

class APPLICATION_API Window
{

public:
	Window( const std::string& name, int width, int height, const WindowParameters &params /**/ );
	virtual ~Window();

	uint32_t width() const;
	uint32_t height() const;


	virtual void onCharEvent(unsigned int)                     {}
	virtual void onCloseEvent()                                {}
	virtual void onCursorEnterEvent(bool)                      {}
	virtual void onCursorPosEvent( double,double)              {}
	virtual void onDropEvent(const std::vector<const char*> &) {}
	virtual void onFocusEvent(bool)                            {}
	virtual void onFramebufferSizeEvent( int w, int h)         {} // <-
	virtual void onIconifyEvent(bool)                          {}
	virtual void onInitializeEvent()                           {}
	virtual void onKeyEvent(/***/)                             {}
	virtual void onMouseButtonEvent(/***/)                     {}
	virtual void onPaintEvent()                                {}
	virtual void onPosEvent( int, int)                         {}
	virtual void onRefreshEvent()                              {}
	virtual void onScrollEvent( double, double)                {}
	
	// ***
	// Do not pass the window size to glViewport or other pixel - based OpenGL calls.
	// The window size is in screen coordinates, not pixels. Use the framebuffer size, which is in pixels, for pixel - based calls.	
	// On some machines screen coordinates and pixels are the same, but on others they will not be.
	virtual void onSizeEvent( int, int)                        {}

protected:
	void makeCurrent();
	void paint();

protected:
	RenderSystem::Context* _context = nullptr;

private:
	void* _handle = nullptr;

	friend class Application;
};

}


#endif